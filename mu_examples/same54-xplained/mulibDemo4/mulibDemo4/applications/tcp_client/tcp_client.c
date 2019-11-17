/**
 * MIT License
 *
 * Copyright (c) 2019 R. Dunbar Poor <rdpoor@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
## On m2m_wifi_handle_events() and busy waits.

ASF's m2m architecture requires that the user continually call
m2m_wifi_handle_events() in order to update its internal state.

This makes it impossible for the processor to sleep or efficiently process other
events in the system.  What we want is a way to know when it's necessary to call
m2m_wifi_handle_events().

There are three user-level callbacks in the m2m system which will only be called
from within a call to m2m_wifi_handle_events():

1. wifi_cb()
2. socket_cb()
3. resolve_cb()

The wifi_cb() may be called from within m2m_wifi_cb(), which in turn is called
(AFAIK) in a call to m2m_wifi_handle_events().  (TODO: verify.) m2m_wifi_cb()
in turn is registered as a callback within m2m_wifi_init():

    hif_register_cb(M2M_REQ_GROUP_WIFI, m2m_wifi_cb);

 So, theoretically, we could register our own callback in its place:

     int main() {
         ...
         wifi_init(&param);
         hif_register_cb(M2M_REQ_GROUP_WIFI, my_wifi_cb);
     }
     static void my_wifi_cb(uint8 u8OpCode, uint16 u16DataSize, uint32 u32Addr)
{ m2m_wifi_cb(u8OpCode, u16DataSize, u32Addr);
       // ... now we know it's time to call m2m_wifi_handle_events():
       // TODO: make sure this is NOT at interrupt level.  If it is, then
       // mu_sched_add_from_isr()...
       mu_sched_add(sched, tcp_task);
   }

Similarly, socket_cb() may be called from m2m_ip_cb, which is in turn registered
as a callback in socketInit():

    hif_register_cb(M2M_REQ_GROUP_IP, m2m_ip_cb);

So, as with wifi, we can intercept it as follows:

  int main() {
      ...
      wifi_init(&param);
      hif_register_cb(M2M_REQ_GROUP_WIFI, my_sock_cb);
  }
  static void my_ip_cb(uint8 u8OpCode, uint16 u16BufferSize, uint32 u32Address)
{ m2m_ip_cb(u8OpCode, u16BufferSize, u32Address);
    // ... now we know it's time to call m2m_wifi_handle_events():
    // TODO: make sure this is NOT at interrupt level.  If it is, then
    // mu_sched_add_from_isr()...
    mu_sched_add(sched, tcp_task);
  }

Finally, what about resolve_cb?  Simple: it's also triggered by m2m_ip_cb, so
setting up the hook for socket_cb() will cover resolve_cb() as well.

## The tcp_task:

A call to tcp_task_fn() should loop as long as the internal state changes,
meaning that progress is being made.  If the state ever stays the same, then we
return because we're waiting on some external event.

 */

// =============================================================================
// includes

#include "tcp_client.h"
#include "driver/include/m2m_wifi.h"
#include "driver/source/m2m_hif.h"
#include "mu_sched.h"
#include "mu_string.h"
#include "mu_task.h"
#include "mu_time.h"
#include "socket/include/socket.h"
#include "winc_init.h"
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// =============================================================================
// local types and definitions

#define IP_ADDR(o1, o2, o3, o4)                                                \
  ((uint32_t)((o1 << 24) | (o2 << 16) | (o3 << 8) | (04 << 0)))

// Define the states for the WiFi callbacks
typedef enum {
  WIFI_STATE_ENTRY,
  WIFI_STATE_DISCONNECTED,
  WIFI_STATE_REQUESTING_DHCP,
  WIFI_STATE_CONNECTED
} wifi_state_t;

// Define the states for the Socket callbacks
typedef enum {
  SOCKET_STATE_ENTRY,
  SOCKET_STATE_CONNECTION_SUCCEEDED,
  SOCKET_STATE_CONNECTION_FAILED,
  SOCKET_STATE_SEND_SUCCEEDED,
  SOCKET_STATE_SEND_FAILED,
  SOCKET_STATE_RECEIVE_SUCCEEDED,
  SOCKET_STATE_RECEIVE_FAILED
} socket_state_t;

#define IP_ADDRESS_UNRESOLVED UINT32_MAX

#define DEFAULT_PORT 80
#define TLS_PORT 443

// The TCP Client structure.  We wrap everything up in a single structure
// rather than have dozens of static variables.
struct tcp_client_ {
  // set up in tcp_client_init()
  mu_sched_t *_sched;
  const char *_hostname; // "https://phaseofthemoon.com" or "96.231.207.26:8080"
  mu_time_seconds_t _timeout; // not yet implemented

  // set up in tcp_client_set_wlan_parmas()
  const char *_wifi_ssid;
  uint8_t _wifi_sec_type;
  const char *_wifi_psk;
  uint16_t _wifi_channel;

  // set up in tcp_client_request()
  mu_task_t *_response_task; // called upon completion of request/ response
  mu_string_t *_request;
  mu_string_t *_response;
  int16_t _response_length;  // set in socket_cb

  //    given a hostname of "https://phaseofthemoon.com:8080":
  mu_string_t _protocol_string; // the protocol substring of hostname ("https")
  mu_string_t _hostname_string; // substring of hostname ("phaseofthemoon.com")

  int _socket;               // socket id
  bool _is_tls;              // true if the user has specified tls
  uint16_t _port;            // port number derived from hostname (8080)
  uint32_t _host_ip_address; // set from hostname or resolve_cb (DNS lookup)

  mu_task_t _request_task;   // main request / response processor
  hcs_t _state;              // request task internal state

  wifi_state_t _wifi_state;     // set in wifi_cb
  socket_state_t _socket_state; // set in socket_cb

  // tpfHifCallBack _orig_wifi_cb;
  // tpfHifCallBack _orig_ip_cb;
};

// =============================================================================
// local (forward) declarations

static tcp_client_t *tcp_client_reset(tcp_client_t *tcp_client);

static tcp_client_err_t parse_hostname(tcp_client_t *tcp_client,
                                        const char *hostname);

static uint32_t parse_ipv4_dotted(mu_string_t *hostname_string);

// static void install_callback_hooks();

static void request_task_fn(void *self_, void *arg);

static hcs_t update_state(hcs_t from, hcs_t to);

static const char *state_name(hcs_t s);

static int tcp_client_socket(tcp_client_t *tcp_client);

static int tcp_client_connect(tcp_client_t *tcp_client);

const char *get_base_hostname(tcp_client_t *host_client);

// static void
// my_wifi_cb(uint8_t u8OpCode, uint16_t u16DataSize, uint32_t u32Addr);

// static void
// my_ip_cb(uint8_t u8OpCode, uint16_t u16BufferSize, uint32_t u32Address);

static void wifi_cb(uint8_t u8MsgType, void *pvMsg);

static void socket_cb(SOCKET sock, uint8_t u8Msg, void *pvMsg);

static void resolve_cb(uint8_t *pu8DomainName, uint32_t u32ServerIP);

static char *format_ipv4_addr(uint32_t ipv4_addr, char *dst, int dst_len);

static char *format_ipv4_octets(uint8_t o1,
                                uint8_t o2,
                                uint8_t o3,
                                uint8_t o4,
                                char *dst,
                                int dst_len);

// =============================================================================
// local storage

// Expand DEFINE_TCS_STATES to generate a table of state names
#undef DEFINE_TCS_STATE
#define DEFINE_TCS_STATE(x) #x,
static const char *s_hcs_names[] = {DEFINE_TCS_STATES};
#undef DEFINE_TCS_STATE

const size_t N_TCS_STATES = sizeof(s_hcs_names) / sizeof(const char *);

// A singleton instance of tcp_client
static tcp_client_t s_tcp_client;

// =============================================================================
// public code

tcp_client_t *tcp_client_get_instance() { return &s_tcp_client; }

tcp_client_err_t tcp_client_init(tcp_client_t *tcp_client,
                                   mu_sched_t *sched,
                                   const char *hostname,
                                   mu_time_seconds_t timeout) {
  tcp_client_err_t err = TCP_CLIENT_ERR_NONE;
  tstrWifiInitParam wifi_params;

  memset(tcp_client, 0, sizeof(tcp_client_t));

  // Initialize wifi with the wifi parameters structure.
  memset((uint8_t *)&wifi_params, 0, sizeof(wifi_params));
  wifi_params.pfAppWifiCb = wifi_cb;
  wifi_init(&wifi_params);

  // Initialize socket module
  tcp_client->_socket = -1;   // no socket opened yet
  socketInit();
  registerSocketCallback(socket_cb, resolve_cb);

  // Now that wifi_init() and socketInit() have ben called, we need to notify
  // the scheduler when low-level wifi or socket changes happen.  To do this we
  // add our hooks to the internal m2m callbacks.
  // install_callback_hooks();

  // Extract host name, port and (optionally) host_ip_addr.
  err = parse_hostname(tcp_client, hostname);
  if (err != TCP_CLIENT_ERR_NONE)
    return err;

  tcp_client->_sched = sched;
  tcp_client->_timeout = timeout;

  tcp_client->_response_task = NULL;
  tcp_client->_request = NULL;
  tcp_client->_response = NULL;

  mu_task_init_immed(&(tcp_client->_request_task),
                     request_task_fn,
                     tcp_client,
                     "TCP Client");

  tcp_client_reset(tcp_client);

  return TCP_CLIENT_ERR_NONE; // could be better!
}

mu_sched_t *tcp_client_get_sched(tcp_client_t *tcp_client) {
  return tcp_client->_sched;
}

const char *tcp_client_get_hostname(tcp_client_t *tcp_client) {
  return tcp_client->_hostname;
}

mu_time_seconds_t tcp_client_get_timeout(tcp_client_t *tcp_client) {
  return tcp_client->_timeout;
}

tcp_client_err_t tcp_client_set_wlan_params(tcp_client_t *tcp_client,
                                              const char *wifi_ssid,
                                              uint8_t wifi_sec_type,
                                              const char *wifi_psk,
                                              uint16_t wifi_channel) {
  tcp_client->_wifi_ssid = wifi_ssid;
  tcp_client->_wifi_sec_type = wifi_sec_type;
  tcp_client->_wifi_psk = wifi_psk;
  tcp_client->_wifi_channel = wifi_channel;

  return TCP_CLIENT_ERR_NONE;
}

mu_task_t *tcp_client_get_response_task(tcp_client_t *tcp_client) {
  return tcp_client->_response_task;
}

mu_string_t *tcp_client_get_request(tcp_client_t *tcp_client) {
  return tcp_client->_request;
}

mu_string_t *tcp_client_get_response(tcp_client_t *tcp_client) {
  return tcp_client->_response;
}

uint16_t tcp_client_get_port(tcp_client_t *tcp_client) {
  return tcp_client->_port;
}

mu_string_t *tcp_client_get_protocol_string(tcp_client_t *tcp_client) {
  return &tcp_client->_protocol_string;
}

mu_string_t *tcp_client_get_hostname_string(tcp_client_t *tcp_client) {
  return &tcp_client->_hostname_string;
}

bool tcp_client_is_tls(tcp_client_t *tcp_client) {
  return tcp_client->_is_tls;
}

uint16_t tcp_client_get_host_ip_address(tcp_client_t *tcp_client) {
  return tcp_client->_host_ip_address;
}

hcs_t tcp_client_get_state(tcp_client_t *tcp_client) {
  return tcp_client->_state;
}

const char *tcp_client_get_state_name(tcp_client_t *tcp_client) {
  hcs_t state = tcp_client->_state;
  if (state < N_TCS_STATES) {
    return s_hcs_names[state];
  } else {
    return "Unknown State";
  }
}


tcp_client_err_t tcp_client_request(tcp_client_t *tcp_client,
                                      mu_string_t *request,
                                      mu_string_t *response,
                                      mu_task_t *response_task) {
  tcp_client->_request = request;
  tcp_client->_response = response;
  tcp_client->_response_task = response_task;

  // schedule the initial call to the tcp client task
  mu_sched_add(tcp_client->_sched, &tcp_client->_request_task);

  return TCP_CLIENT_ERR_NONE;
}

// =============================================================================
// local (static) code

static tcp_client_t *tcp_client_reset(tcp_client_t *tcp_client) {
  tcp_client->_state = TCS_ENTRY;
  tcp_client->_wifi_state = WIFI_STATE_ENTRY;
  tcp_client->_socket_state = SOCKET_STATE_ENTRY;

  return tcp_client;
}

/**
 * @brief parse hostname to extract host name and port.
 *
 * Parses the hostname argument:
 *
 *   [<protocol>://]<hostname>[:port]
 *
 * <hostname> can be IPv4 dotted octets, such as `32.67.1.15` or a string naming
 * a host such as `www.nike.com`.
 *
 * If protocol is given and equals `https`, the default port is set to 443.  If
 * no protocol is specified, or if protocol equals `http`, the default port is
 * set to 80.
 *
 * If port is given, it overrides any default port specified by the protocol.
 *
 * @param tcp_client The tcp client object
 * @param hostname The hostname
 *
 * Separately:
 * - If hostname conforms to the IPV4 dotted address format, the host_ip_addr
 *   field is set to that number
 * - Otherwise the hostname (stripped of protocol prefix and port suffix) will
 *   be passed to gethostbyname() to resolve the host_ip_addr.
 */
static tcp_client_err_t parse_hostname(tcp_client_t *tcp_client,
                                        const char *hostname) {
  mu_string_t *protocol_string = &tcp_client->_protocol_string;
  mu_string_t *hostname_string = &tcp_client->_hostname_string;
  mu_string_t temp_string;

  // some defaults
  tcp_client->_port = DEFAULT_PORT;
  tcp_client->_is_tls = false;
  tcp_client->_host_ip_address = IP_ADDRESS_UNRESOLVED;
  mu_string_init(protocol_string, (char *)hostname, strlen(hostname));
  mu_string_init(hostname_string, (char *)hostname, strlen(hostname));
  // TODO: find a better way to reason with C strings
  mu_string_set_length(hostname_string, mu_string_capacity(hostname_string));

  mu_string_init(&temp_string, (char *)hostname, strlen(hostname));

  if (mu_string_find(&temp_string, "://", NULL)) {
    // found :// protocol prefix.  capture string before :// in protocol_string
    // and capture string following :// in hostname_string
    mu_string_slice(protocol_string, 0, mu_string_start(&temp_string), NULL);
    mu_string_slice(hostname_string, mu_string_end(&temp_string), -1, NULL);
  }

  // search within hostname_string, store results in temp_string
  if (mu_string_find(hostname_string, ":", &temp_string)) {
    mu_string_slice(hostname_string,
                    mu_string_start(hostname_string),
                    mu_string_start(&temp_string),
                    NULL);
    mu_string_slice(&temp_string, mu_string_end(&temp_string), -1, NULL);
  } else {
    // no port specified
    mu_string_reset(&temp_string);
  }

  // At this point:
  // protocol_string delineates a protocol ('https') or is empty
  // hostname_string delineates a hostname ('flogostronlabs.com')
  // temp_string delineates a port ('8080') or is empty

  // TODO: implement mu_string_icmp for case-independent comparison
  if ((mu_string_length(protocol_string) > 0) && (mu_string_cmp(protocol_string, "https")) == 0) {
    tcp_client->_port = TLS_PORT;
    tcp_client->_is_tls = true; // abstraction violation!  but justified.
  }

  if (mu_string_length(&temp_string) != 0) {
    // port specifier found following ':'
    uint16_t port = 0;
    for (int i = mu_string_start(&temp_string); i < mu_string_end(&temp_string);
         i++) {
      char c = mu_string_buf(&temp_string)[i];
      if (isdigit(c)) {
        return TCP_CLIENT_ERR_BAD_FORMAT;
      } else {
        port *= 10;
        port += c - '0';
      }
    }
    tcp_client->_port = port;
    if (port == TLS_PORT) {
      tcp_client->_is_tls = true;
    }
  }

  // see if the hostname is already in IPv4 dotted-decimal format.  If so,
  // return 32bit value otherwise return IP_ADDRESS_UNRESOLVED.
  tcp_client->_host_ip_address = parse_ipv4_dotted(hostname_string);

  return TCP_CLIENT_ERR_NONE;
}

static uint32_t parse_ipv4_dotted(mu_string_t *hostname_string) {
  char buf[16];
  mu_string_extract(hostname_string, buf, 16); // get null-terminated string
  uint32_t addr = nmi_inet_addr(buf);
  return addr == 0 ? IP_ADDRESS_UNRESOLVED : addr;
}

// static void install_callback_hooks() {
//   // Install our callback in place of m2m_wifi_cb.  But since m2m_wifi_cb is a
//   // static function, we access the function pointer instead.
//   if (s_tcp_client.orig_wifi_cb != gstrHifCxt.pfWifiCb) {
//     s_tcp_client.orig_wifi_cb = gstrHifCxt.pfWifiC;
//     hif_register_cb(M2M_REQ_GROUP_WIFI, my_wifi_cb);
//   }
//   // Install our callback in place of m2m_ip_cb.  But since m2m_ip_cb is a
//   // static function, we need to access the function pointer instead.
//   if (s_tcp_client.orig_ip_cb != gstrHifCxt.pfIpCb) {
//     s_tcp_client.orig_ip_cb = gstrHifCxt.pfIpC;
//     hif_register_cb(M2M_REQ_GROUP_IP, my_ip_cb);
//   }
// }
//
// static void my_wifi_cb(uint8 u8OpCode, uint16 u16DataSize, uint32 u32Addr) {
//   // call the original m2m_wifi_cb before running our client task
//   s_tcp_client.orig_wifi_cb(u8OpCode, u16DataSize, u32Addr);
//   mu_sched_add(s_tcp_client.sched, &s_tcp_client.request_task);
// }
//
// static void my_ip_cb(uint8 u8OpCode, uint16 u16BufferSize, uint32 u32Address) {
//   // call the original m2m_ip_cb before running our client task
//   s_tcp_client.orig_ip_cb(u8OpCode, u16BufferSize, u32Address);
//   mu_sched_add(s_tcp_client.sched, &s_tcp_client.request_task);
// }

// The function for the tcp_request task.  Initially triggered by a call to
// tcp_client_request(), subsequently re-schedules itself as until it reaches
// an end state
//
// This runs a state machine that tracks the lifetimes of the WiFi connection
// and the socket connection.
static void request_task_fn(void *self, void *arg) {
  tcp_client_t *tcp_client = (tcp_client_t *)self;
  hcs_t next_state = tcp_client->_state; //
  hcs_t curr_state;                       //
  bool completed = false;                 // true for terminal states.

  do {
    // process network controller events first: this may cause one of the callback
    // functions (wifi_cb, socket_cb or resolve_cb) to be triggered.
    m2m_wifi_handle_events(NULL);

    curr_state = next_state;

    switch (curr_state) {
    case TCS_ENTRY:
      next_state = update_state(curr_state, TCS_INITIATING_AP_CONNECTION);
      break;

    case TCS_INITIATING_AP_CONNECTION:
      if (tcp_client->_wifi_state != WIFI_STATE_CONNECTED) {
        // Request a connection with the WiFi Access Point
        m2m_wifi_connect((char *)tcp_client->_wifi_ssid,
                         strlen(tcp_client->_wifi_ssid),
                         tcp_client->_wifi_sec_type,
                         (char *)tcp_client->_wifi_psk,
                         tcp_client->_wifi_channel);
        next_state = update_state(curr_state, TCS_AWAITING_AP_CONNECTION);
      } else {
        // Already connected to the AP.  Skip ahead to DNS resolution
        next_state = update_state(curr_state, TCS_INITIATING_DNS_RESOLUTION);
      }
      break;

    case TCS_AWAITING_AP_CONNECTION:
      // Here, we've previously requested a connection to the AP
      if (tcp_client->_wifi_state == WIFI_STATE_CONNECTED) {
        // wifi_cb has indicated that the connection is made
        next_state = update_state(curr_state, TCS_INITIATING_DNS_RESOLUTION);
      } else if (tcp_client->_wifi_state == WIFI_STATE_DISCONNECTED) {
        // wifi_cb has indicated a problem creating the connection.
        next_state = update_state(curr_state, TCS_AP_CONNECT_FAILURE);
      } else {
        // no change yet: keep waiting for notification from wifi_cb
      }
      break;

    case TCS_INITIATING_DNS_RESOLUTION:
      if (tcp_client->_host_ip_address != IP_ADDRESS_UNRESOLVED) {
        // if we already have a server IP address we can skip DNS
        next_state = update_state(curr_state, TCS_INITIATING_SOCKET_CONNECTION);

      } else if (gethostbyname((uint8_t *)get_base_hostname(tcp_client)) ==
                 SOCK_ERR_NO_ERROR) {
        // We have initiazed a request for DNS resolution.
        next_state = update_state(curr_state, TCS_AWAITING_DNS_RESOLUTION);

      } else {
        // call to gethostbyname() failed.
        next_state = update_state(curr_state, TCS_GETHOSTBYNAME_FAILURE);
      }
      break;

    case TCS_AWAITING_DNS_RESOLUTION:
      // Arrive here after making a call to gethostbyname().
      if (tcp_client->_host_ip_address == IP_ADDRESS_UNRESOLVED) {
        // no change: still awaiting resolve_cb to trigger.
        // TODO: is there a way to detect timeout while waiting for DNS?

      } else if (tcp_client->_host_ip_address == 0) {
        // DNS failed to resolve the address
        next_state = update_state(curr_state, TCS_DNS_RESOLUTION_FAILURE);

      } else {
        // resolve_cb has triggered and given us the server ip address
        next_state = update_state(curr_state, TCS_INITIATING_SOCKET_CONNECTION);
      }
      break;

    case TCS_INITIATING_SOCKET_CONNECTION:
      if (tcp_client_socket(tcp_client) < 0) {
        // failed to create TCP client socket
        next_state = update_state(curr_state, TCS_SOCKET_FAILURE);
      } else if (tcp_client_connect(tcp_client) < 0) {
        // failed to connect TCP client socket.
        next_state = update_state(curr_state, TCS_CONNECT_FAILURE);
      } else {
        // successfully created and connected socket
        next_state = update_state(curr_state, TCS_AWAITING_SOCKET_CONNECTION);
      }
      break;

    case TCS_AWAITING_SOCKET_CONNECTION:
      if (tcp_client->_socket_state == SOCKET_STATE_CONNECTION_SUCCEEDED) {
        next_state = update_state(curr_state, TCS_INITIATING_SEND);
      } else if (tcp_client->_socket_state == SOCKET_STATE_CONNECTION_FAILED) {
        next_state = update_state(curr_state, TCS_CONNECT_FAILURE);
      } else {
        // no change yet: keep waiting for notification from socket_cb
      }
      break;

    case TCS_INITIATING_SEND:
      printf("--->>> sending:\r\n%s\r\n------\r\n",
             mu_string_cstring(tcp_client->_request));
      send(tcp_client->_socket,
           (void *)mu_string_cstring(tcp_client->_request),
           mu_string_length(tcp_client->_request),
           0);
      next_state = update_state(curr_state, TCS_AWAITING_SEND_CONFIRMATION);
      break;

    case TCS_AWAITING_SEND_CONFIRMATION:
      if (tcp_client->_socket_state == SOCKET_STATE_SEND_SUCCEEDED) {
        next_state = update_state(curr_state, TCS_INITIATING_RECEIVE);
      } else if (tcp_client->_socket_state == SOCKET_STATE_SEND_FAILED) {
        next_state = update_state(curr_state, TCS_SEND_FAILURE);
      } else {
        // no change yet: keep waiting for notification from socket_cb
      }
      break;

    case TCS_INITIATING_RECEIVE:
      recv(tcp_client->_socket,
           mu_string_cstring(tcp_client->_response),
           mu_string_length(tcp_client->_response),
           0);
      next_state = update_state(curr_state, TCS_AWAITING_RECEIVE_CONFIRMATION);
      break;

    case TCS_AWAITING_RECEIVE_CONFIRMATION:
      if (tcp_client->_socket_state == SOCKET_STATE_RECEIVE_SUCCEEDED) {
        printf("<<<--- received:\r\n%s\r\n------\r\n", mu_string_cstring(tcp_client->_response));
        mu_string_set_length(tcp_client->_response, tcp_client->_response_length);
        next_state = update_state(curr_state, TCS_SUCCESS);
      } else if (tcp_client->_socket_state == SOCKET_STATE_RECEIVE_FAILED) {
        next_state = update_state(curr_state, TCS_RECEIVE_FAILURE);
      } else {
        // no change yet: keep waiting for notification from socket_cb
      }
      break;

    case TCS_RECEIVE_FAILURE:
    case TCS_SEND_FAILURE:
    case TCS_CONNECT_FAILURE:
    case TCS_SOCKET_FAILURE:
      close(tcp_client->_socket);
      tcp_client->_socket = -1;
      tcp_client->_socket_state = SOCKET_STATE_ENTRY;
      completed = true;
      break;

    case TCS_DNS_RESOLUTION_FAILURE:
    case TCS_GETHOSTBYNAME_FAILURE:
    case TCS_AP_CONNECT_FAILURE:
      tcp_client->_wifi_state = WIFI_STATE_ENTRY;
      completed = true;
      break;

    case TCS_SUCCESS:
      // success.
      // This may not be the best way, but we want to keep using the socket.
      tcp_client->_socket_state = SOCKET_STATE_CONNECTION_SUCCEEDED;
      completed = true;
      break;

    } // switch
  } while ((next_state != curr_state) && (completed == false));

  tcp_client->_state = next_state;  // Don't forget to de-cache the state!  (Doh!)

  // Arrive here because state did not advance or completed was set to true.
  // If completed was set to true, it's time to call the user's callback.
  // If the state did not advance, it means we should wait on an external event
  // (wifi callback, socket callback, etc) to reschedule this task.

  if (completed) {
    mu_task_call(tcp_client->_response_task, self);
  } else {
    // until we figure out how to get an external event to retrigger this event,
    // re-trigger in 100 milliseconds.
    mu_task_set_time(&tcp_client->_request_task,
                     mu_time_offset(mu_time_now(),
                                    mu_time_seconds_to_duration(0.1)));
    mu_sched_add(tcp_client->_sched, &tcp_client->_request_task);
  }
}

static hcs_t update_state(hcs_t from, hcs_t to) {
  printf("%s => %s\r\n", state_name(from), state_name(to));

  // state_telemetry_t *t_from = &state_telemetry[from];
  // state_telemetry_t *t_to = &state_telemetry[to];
  // Work in progress: The SAME54 has a DWT clock: search for
  //    DWT_CONTROL  = (int *)0xE0001000; //address of the register
  // and related functions...
  // time_t now = get_time();
  // time_dt dur = now - t_from->start;
  // t_from->ncalls += 1;
  // t_from->t_accum += dur;
  // if (dur > t_from->t_max) t_from->t_max = dur;
  //
  // t_to->start = now;

  return to;
}

static const char *state_name(hcs_t s) { return s_hcs_names[s]; }

/**
 * Extract the basic hostname with protocol and port number stripped off, e.g.
 *   "https://example.com:8080" => "example.com"
 */
const char *get_base_hostname(tcp_client_t *tcp_client) {
  static char hostname[HOSTNAME_MAX_SIZE];
  mu_string_extract(&tcp_client->_hostname_string, hostname, HOSTNAME_MAX_SIZE);
  return (const char *)hostname;
}

/**
 * Call socket() if needed.
 */
static int tcp_client_socket(tcp_client_t *tcp_client) {
  if (tcp_client->_socket < 0) {
    // need to get a socket #
    uint8_t flags = tcp_client->_is_tls ? SOCKET_FLAGS_SSL : 0;
    tcp_client->_socket = socket(AF_INET, SOCK_STREAM, flags);
  }
  return tcp_client->_socket;
}

static int tcp_client_connect(tcp_client_t *tcp_client) {
  struct sockaddr_in sockaddr;
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_port = _htons(tcp_client->_port);
  sockaddr.sin_addr.s_addr = tcp_client->_host_ip_address;

  return connect(
      tcp_client->_socket, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
}

/**
 * \brief Callback to get the Wi-Fi status update.
 *
 * \param[in] u8MsgType type of Wi-Fi notification. Possible types are:
 *  - [M2M_WIFI_RESP_CURRENT_RSSI](@ref M2M_WIFI_RESP_CURRENT_RSSI)
 *  - [M2M_WIFI_RESP_CON_STATE_CHANGED](@ref M2M_WIFI_RESP_CON_STATE_CHANGED)
 *  - [M2M_WIFI_RESP_CONNTION_STATE](@ref M2M_WIFI_RESP_CONNTION_STATE)
 *  - [M2M_WIFI_RESP_SCAN_DONE](@ref M2M_WIFI_RESP_SCAN_DONE)
 *  - [M2M_WIFI_RESP_SCAN_RESULT](@ref M2M_WIFI_RESP_SCAN_RESULT)
 *  - [M2M_WIFI_REQ_WPS](@ref M2M_WIFI_REQ_WPS)
 *  - [M2M_WIFI_RESP_IP_CONFIGURED](@ref M2M_WIFI_RESP_IP_CONFIGURED)
 *  - [M2M_WIFI_RESP_IP_CONFLICT](@ref M2M_WIFI_RESP_IP_CONFLICT)
 *  - [M2M_WIFI_RESP_P2P](@ref M2M_WIFI_RESP_P2P)
 *  - [M2M_WIFI_RESP_AP](@ref M2M_WIFI_RESP_AP)
 *  - [M2M_WIFI_RESP_CLIENT_INFO](@ref M2M_WIFI_RESP_CLIENT_INFO)
 * \param[in] pvMsg A pointer to a buffer containing the notification parameters
 * (if any). It should be casted to the correct data type corresponding to the
 * notification type. Existing types are:
 *  - tstrM2mWifiStateChanged
 *  - tstrM2MWPSInfo
 *  - tstrM2MP2pResp
 *  - tstrM2MAPResp
 *  - tstrM2mScanDone
 *  - tstrM2mWifiscanResult
 */
static void wifi_cb(uint8_t u8MsgType, void *pvMsg) {
  tcp_client_t *tcp_client = &s_tcp_client;

  switch (u8MsgType) {

  case M2M_WIFI_RESP_CON_STATE_CHANGED: {
    tstrM2mWifiStateChanged *pstrWifiState = (tstrM2mWifiStateChanged *)pvMsg;

    if (pstrWifiState->u8CurrState == M2M_WIFI_CONNECTED) {
      printf("wifi_cb: M2M_WIFI_RESP_CON_STATE_CHANGED: CONNECTED\r\n");
      tcp_client->_wifi_state = WIFI_STATE_REQUESTING_DHCP;
      m2m_wifi_request_dhcp_client();

    } else if (pstrWifiState->u8CurrState == M2M_WIFI_DISCONNECTED) {
      printf("wifi_cb: M2M_WIFI_RESP_CON_STATE_CHANGED: DISCONNECTED\r\n");
      tcp_client->_wifi_state = WIFI_STATE_DISCONNECTED;
    }
  } break;

  case M2M_WIFI_REQ_DHCP_CONF: {
    uint8_t *pu8IPAddress = (uint8_t *)pvMsg;
    char buf[16];
    tcp_client->_wifi_state = WIFI_STATE_CONNECTED;
    format_ipv4_octets(pu8IPAddress[0],
                       pu8IPAddress[1],
                       pu8IPAddress[2],
                       pu8IPAddress[3],
                       buf,
                       16);
    printf("wifi_cb: M2M_WIFI_REQ_DHCP_CONF: IP is %s\r\n", buf);
  } break;

  default:
    break;
  }
}

/**
 * \brief Callback to get the Data from socket.
 *
 * \param[in] sock socket handler.
 * \param[in] u8Msg socket event type. Possible values are:
 *  - SOCKET_MSG_BIND
 *  - SOCKET_MSG_LISTEN
 *  - SOCKET_MSG_ACCEPT
 *  - SOCKET_MSG_CONNECT
 *  - SOCKET_MSG_RECV
 *  - SOCKET_MSG_SEND
 *  - SOCKET_MSG_SENDTO
 *  - SOCKET_MSG_RECVFROM
 * \param[in] pvMsg is a pointer to message structure. Existing types are:
 *  - tstrSocketBindMsg
 *  - tstrSocketListenMsg
 *  - tstrSocketAcceptMsg
 *  - tstrSocketConnectMsg
 *  - tstrSocketRecvMsg
 */
static void socket_cb(SOCKET sock, uint8_t u8Msg, void *pvMsg) {
  tcp_client_t *tcp_client = &s_tcp_client;

  switch (u8Msg) {
  /* Socket connected */
  case SOCKET_MSG_CONNECT: {
    tstrSocketConnectMsg *pstrConnect = (tstrSocketConnectMsg *)pvMsg;
    if (pstrConnect && pstrConnect->s8Error >= 0) {
      printf("socket_cb: connect success!\r\n");
      tcp_client->_socket_state = SOCKET_STATE_CONNECTION_SUCCEEDED;
    } else {
      printf("socket_cb: connect error!\r\n");
      tcp_client->_socket_state = SOCKET_STATE_CONNECTION_FAILED;
    }
  } break;

  /* Message send */
  case SOCKET_MSG_SEND: {
    printf("socket_cb: send success!\r\n");
    tcp_client->_socket_state = SOCKET_STATE_SEND_SUCCEEDED;
  } break;

  /* Message receive */
  case SOCKET_MSG_RECV: {
    tstrSocketRecvMsg *pstrRecv = (tstrSocketRecvMsg *)pvMsg;
    if (pstrRecv && pstrRecv->s16BufferSize > 0) {
      printf("socket_cb: recv success!\r\n");
      tcp_client->_socket_state = SOCKET_STATE_RECEIVE_SUCCEEDED;
      // note how many bytes were received
      tcp_client->_response_length = pstrRecv->s16BufferSize;

    } else {
      printf("socket_cb: recv error!\r\n");
      tcp_client->_socket_state = SOCKET_STATE_RECEIVE_FAILED;
    }
  } break;

  default:
    break;
  }
}

/**
 * @brief DNS resolution callback function.
 *
 * Applications requiring DNS resolution should register their callback through
 * this function by calling @ref registerSocketCallback. The following callback
 * is triggered in response to asynchronous call to the @ref gethostbyname
 * function (DNS Resolution callback).
 *
 * Note: this function conforms to the tpfAppResolveCb signature format, which
 * is mentioned here because it might help you find more info in the ASF
 * documentation.
 *
 * @param [in] pu8DomainName Domain name of the host.
 *
 * @param [in]	u32ServerIP Server IPv4 address encoded in NW byte order format.
 * If it is 0, then the DNS resolution failed.
 */
static void resolve_cb(uint8_t *pu8DomainName, uint32_t u32ServerIP) {
  tcp_client_t *tcp_client = &s_tcp_client;
  char buf[16];

  format_ipv4_addr(u32ServerIP, buf, sizeof(buf));
  printf("resolve_cb: %s => %s\r\n", pu8DomainName, buf);
  tcp_client->_host_ip_address = u32ServerIP;
}

/**
 * @brief Convert a 32 bit IPV4 address into a string.
 *
 * Note: dst_len should be at least 16 bytes long
 */
static char *format_ipv4_addr(uint32_t ipv4_addr, char *dst, int dst_len) {
  return format_ipv4_octets((ipv4_addr >> 24) & 0xff,
                            (ipv4_addr >> 16) & 0xff,
                            (ipv4_addr >> 8) & 0xff,
                            (ipv4_addr >> 0) & 0xff,
                            dst,
                            dst_len);
}

/**
 * @brief Convert four 8 bit IPV4 address octets into a string.
 *
 * Note: dst_len should be at least 16 bytes long
 */
static char *format_ipv4_octets(uint8_t o1,
                                uint8_t o2,
                                uint8_t o3,
                                uint8_t o4,
                                char *dst,
                                int dst_len) {
  snprintf(dst, dst_len, "%u.%u.%u.%u", o1, o2, o3, o4);
  return dst;
}
