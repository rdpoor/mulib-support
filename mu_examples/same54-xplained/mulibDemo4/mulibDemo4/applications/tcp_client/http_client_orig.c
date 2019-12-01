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

The wifi_cb() may be called from within m2m_wifi_cb().   m2m_wifi_cb() in turn
is registered as a callback within mem_wifi_init():

    hif_register_cb(M2M_REQ_GROUP_WIFI, m2m_wifi_cb);

 So, theoretically, we could register our own callback in its place:

     int main() {
         ...
         wifi_init(&param);
         hif_register_cb(M2M_REQ_GROUP_WIFI, my_wifi_cb);
     }
     static void my_wifi_cb(uint8 u8OpCode, uint16 u16DataSize, uint32 u32Addr) {
       m2m_wifi_cb(u8OpCode, u16DataSize, u32Addr);
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


However, it should be possible to intercept a low-level
event (or two) to avoid this problem.

The function socket_init() internally calls


 */

// =============================================================================
// includes

#include "tcp_client.h"

// =============================================================================
// local types and definitions

// Define the states for the WiFi callbacks
typedef enum {
  WIFI_STATE_ENTRY,
  WIFI_STATE_DISCONNECTED,
  WIFI_STATE_REQUESTING_DHCP,
  WIFI_STATE_CONNECTED
} wifi_state_t;

// Define the states for the Socket callbacks
typedef enum {
  SSTATE_ENTRY,
  SSTATE_CONNECTION_SUCCEEDED,
  SSTATE_CONNECTION_FAILED,
  SSTATE_SEND_SUCCEEDED,
  SSTATE_SEND_FAILED,
  SSTATE_RECEIVE_SUCCEEDED,
  SSTATE_RECEIVE_FAILED
} socket_state_t;

// =============================================================================
// local (forward) declarations

static void tcp_task_fn(void *self_, void *arg);

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
static void wifi_cb(uint8_t u8MsgType, void *pvMsg);

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
static void socket_cb(SOCKET sock, uint8_t u8Msg, void *pvMsg);

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
 * If it is Zero, then the DNS resolution failed.
 */
static void resolve_cb(uint8_t *pu8DomainName, uint32_t u32ServerIP);

static void install_callback_hooks();


// =============================================================================
// local storage

// Expand DEFINE_HCS_STATES to generate a table of state names
#undef DEFINE_HCS_STATE
#define DEFINE_HCS_STATE(x) #x,
static const char *s_tcp_task_state_names[] = {DEFINE_HCS_STATES};

#undef DEFINE_HCS_STATE

const size_t N_HCS_STATES = sizeof(s_state_names) / sizeof(const char *);

// =============================================================================
// public code

tcp_task_t *tcp_task_init(tcp_task_t *tcp_task, mu_sched_t *sched) {
  tcp_task->sched = sched;
  mu_task_init(&(tcp_task->task), tcp_task);
  tcp_task->completion_cb = NULL;
  tcp_task->server_name = NULL;
  return tcp_task_reset(tcp_task);
}

tcp_task_t *tcp_task_reset(tcp_task_t *tcp_task) {
  tcp_task->state = TSTATE_ENTRY;
  return tcp_task;
}

tcp_task_t *tcp_task_start_exchange(mu_tcp_task_t *tcp_task,
                                      uint8_t *url,
                                      mu_string_t *header,
                                      mu_string_t *req_body,
                                      mu_string_t *rsp_body,
                                      mu_task_t *completion_cb) {
  mu_sched_add(tcp_task->sched, &(tcp_task->task));
  return tcp_task;
}

tcp_task_state_t tcp_task_get_state(tcp_task_t *tcp_task) {
  return tcp_task->state;
}

const char *tcp_task_state_name(tcp_task_state_t state) {
  return s_state_names[s];
}

// =============================================================================
// local (static) code

static void tcp_task_fn(void *self_, void *arg) {
  tcp_task_t *self = (tcp_task_t *)self_;
  http_sched_t *sched = (http_sched_t *)arg;
  tcp_task_state_t s0 = self->state; // state upon entry
  tcp_task_state_t s1 = s0;          // state upon exit
  bool completed = false;

  // process network controller events first: this may cause one of the callback
  // functions (wifi_cb, socket_cb or resolve_cb) to be triggered.
  m2m_wifi_handle_events(NULL);

  do {
    s0 = s1;

    switch (s0) {
    case TSTATE_ENTRY:
      s1 = update_state(s0, TSTATE_INITIATING_AP_CONNECTION);
      break;

    case TSTATE_INITIATING_AP_CONNECTION:
      if (s_wifi_state != WIFI_STATE_CONNECTED) {
        // Request a connection with the WiFi Access Point
        m2m_wifi_connect((char *)MAIN_WLAN_SSID,
                         sizeof(MAIN_WLAN_SSID),
                         MAIN_WLAN_AUTH,
                         (char *)MAIN_WLAN_PSK,
                         M2M_WIFI_CH_ALL);
        s1 = update_state(s0, TSTATE_AWAITING_AP_CONNECTION);
      } else {
        // Already connected to the AP.  Skip ahead to DNS resolution
        s1 = update_state(s0, TSTATE_INITIATING_DNS_RESOLUTION);
      }
      break;

    case TSTATE_AWAITING_AP_CONNECTION:
      // Here, we've previously requested a connection to the AP
      if (s_wifi_state == WIFI_STATE_CONNECTED) {
        // wifi_cb has indicated that the connection is made
        s1 = update_state(s0, TSTATE_INITIATING_DNS_RESOLUTION);
      } else if (s_wifi_state == WIFI_STATE_DISCONNECTED) {
        // wifi_cb has indicated a problem creating the connection.
        s1 = update_state(s0, TSTATE_AP_CONNECT_FAILURE);
      } else {
        // no change yet: keep waiting for notification from wifi_cb
      }
      break;

    case TSTATE_INITIATING_DNS_RESOLUTION:
      if (s_server_ip_addr != IP_ADDRESS_UNRESOLVED) {
        // if we already have a server IP address we can skip DNS
        s1 = update_state(s0, TSTATE_INITIATING_SOCKET_CONNECTION);

      } else if (gethostbyname((uint8_t *)SERVER_NAME) == SOCK_ERR_NO_ERROR) {
        // We have initiazed a request for DNS resolution.
        s1 = update_state(s0, TSTATE_AWAITING_DNS_RESOLUTION);

      } else {
        // call to gethostbyname() failed.
        s1 = update_state(s0, TSTATE_GETHOSTBYNAME_FAILURE);
      }
      break;

    case TSTATE_AWAITING_DNS_RESOLUTION:
      // Arrive here after making a call to gethostbyname().
      if (s_server_ip_addr == IP_ADDRESS_UNRESOLVED) {
        // no change: still awaiting resolve_cb to trigger.
        // TODO: is there a way to detect timeout while waiting for DNS?

      } else if (s_server_ip_addr == 0) {
        // DNS failed to resolve the address
        s1 = update_state(s0, TSTATE_DNS_RESOLUTION_FAILURE);

      } else {
        // resolve_cb has triggered and given us the server ip address
        addr->sin_addr.s_addr = s_server_ip_addr;
        s1 = update_state(s0, TSTATE_INITIATING_SOCKET_CONNECTION);
      }
      break;

    case TSTATE_INITIATING_SOCKET_CONNECTION:
      if (tcp_client_socket < 0) {
        // need to connect: initialize socket address structure.
        if ((tcp_client_socket = socket(AF_INET, SOCK_STREAM, flags)) < 0) {
          // failed to create TCP client socket
          s1 = update_state(s0, TSTATE_SOCKET_FAILURE);
        } else if (connect(tcp_client_socket,
                           (struct sockaddr *)addr,
                           sizeof(struct sockaddr_in)) < 0) {
          // failed to connect TCP client socket.
          s1 = update_state(s0, TSTATE_CONNECT_FAILURE);
        } else {
          // successfully created and connected socket
          s1 = update_state(s0, TSTATE_AWAITING_SOCKET_CONNECTION);
        }
      } else {
        // already have a socket connection
        s1 = update_state(s0, TSTATE_AWAITING_SOCKET_CONNECTION);
      }
      break;

    case TSTATE_AWAITING_SOCKET_CONNECTION:
      if (s_socket_state == SSTATE_CONNECTION_SUCCEEDED) {
        s1 = update_state(s0, TSTATE_INITIATING_SEND);
      } else if (s_socket_state == SSTATE_CONNECTION_FAILED) {
        s1 = update_state(s0, TSTATE_CONNECT_FAILURE);
      } else {
        // no change yet: keep waiting for notification from socket_cb
      }
      break;

    case TSTATE_INITIATING_SEND:
      printf("--->>> sending:\r\n%s\r\n------\r\n", req);
      send(tcp_client_socket, (void *)req, strlen(req), 0);
      s1 = update_state(s0, TSTATE_AWAITING_SEND_CONFIRMATION);
      break;

    case TSTATE_AWAITING_SEND_CONFIRMATION:
      if (s_socket_state == SSTATE_SEND_SUCCEEDED) {
        s1 = update_state(s0, TSTATE_INITIATING_RECEIVE);
      } else if (s_socket_state == SSTATE_SEND_FAILED) {
        s1 = update_state(s0, TSTATE_SEND_FAILURE);
      } else {
        // no change yet: keep waiting for notification from socket_cb
      }
      break;

    case TSTATE_INITIATING_RECEIVE:
      recv(tcp_client_socket, (void *)rsp, rsp_size, 0);
      s1 = update_state(s0, TSTATE_AWAITING_RECEIVE_CONFIRMATION);
      break;

    case TSTATE_AWAITING_RECEIVE_CONFIRMATION:
      if (s_socket_state == SSTATE_RECEIVE_SUCCEEDED) {
        printf("<<<--- received:\r\n%s\r\n------\r\n", rsp);
        s1 = update_state(s0, TSTATE_SUCCESS);
      } else if (s_socket_state == SSTATE_RECEIVE_FAILED) {
        s1 = update_state(s0, TSTATE_RECEIVE_FAILURE);
      } else {
        // no change yet: keep waiting for notification from socket_cb
      }
      break;

    case TSTATE_RECEIVE_FAILURE:
    case TSTATE_SEND_FAILURE:
    case TSTATE_CONNECT_FAILURE:
    case TSTATE_SOCKET_FAILURE:
      close(tcp_client_socket);
      tcp_client_socket = -1;
      s_socket_state = SSTATE_ENTRY;
      completed = true;
      break;

    case TSTATE_DNS_RESOLUTION_FAILURE:
    case TSTATE_GETHOSTBYNAME_FAILURE:
    case TSTATE_AP_CONNECT_FAILURE:
      s_wifi_state = WIFI_STATE_ENTRY;
      completed = true;
      break;

    case TSTATE_SUCCESS:
      // success.
      // This may not be the best way, but we want to keep using the socket.
      s_socket_state = SSTATE_CONNECTION_SUCCEEDED;
      completed = true;
      break;

    } // switch
  } while ((s1 != s0) && (completed == false));

  // Arrive here because state did not advance or completed was set to true.
  // If completed was set to true, it's time to call the user's callback.
  // If the state did not advance, it means we're waiting on some external event
  // (wifi callback, socket callback, etc) to reschedule this task.

  if (completed) {
    mu_task_call(self->completion_cb, self);
  }
}

static void wifi_cb(uint8_t u8MsgType, void *pvMsg) {
  switch (u8MsgType) {

  case M2M_WIFI_RESP_CON_STATE_CHANGED: {
    tstrM2mWifiStateChanged *pstrWifiState = (tstrM2mWifiStateChanged *)pvMsg;

    if (pstrWifiState->u8CurrState == M2M_WIFI_CONNECTED) {
      printf("wifi_cb: M2M_WIFI_RESP_CON_STATE_CHANGED: CONNECTED\r\n");
      s_wifi_state = WIFI_STATE_REQUESTING_DHCP;
      m2m_wifi_request_dhcp_client();

    } else if (pstrWifiState->u8CurrState == M2M_WIFI_DISCONNECTED) {
      printf("wifi_cb: M2M_WIFI_RESP_CON_STATE_CHANGED: DISCONNECTED\r\n");
      s_wifi_state = WIFI_STATE_DISCONNECTED;
    }
  } break;

  case M2M_WIFI_REQ_DHCP_CONF: {
    uint8_t *pu8IPAddress = (uint8_t *)pvMsg;
    char buf[16];
    s_wifi_state = WIFI_STATE_CONNECTED;
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

static void socket_cb(SOCKET sock, uint8_t u8Msg, void *pvMsg) {
  switch (u8Msg) {
  /* Socket connected */
  case SOCKET_MSG_CONNECT: {
    tstrSocketConnectMsg *pstrConnect = (tstrSocketConnectMsg *)pvMsg;
    if (pstrConnect && pstrConnect->s8Error >= 0) {
      printf("socket_cb: connect success!\r\n");
      s_socket_state = SSTATE_CONNECTION_SUCCEEDED;
    } else {
      printf("socket_cb: connect error!\r\n");
      s_socket_state = SSTATE_CONNECTION_FAILED;
    }
  } break;

  /* Message send */
  case SOCKET_MSG_SEND: {
    printf("socket_cb: send success!\r\n");
    s_socket_state = SSTATE_SEND_SUCCEEDED;
  } break;

  /* Message receive */
  case SOCKET_MSG_RECV: {
    tstrSocketRecvMsg *pstrRecv = (tstrSocketRecvMsg *)pvMsg;
    if (pstrRecv && pstrRecv->s16BufferSize > 0) {
      printf("socket_cb: recv success!\r\n");
      s_socket_state = SSTATE_RECEIVE_SUCCEEDED;

    } else {
      printf("socket_cb: recv error!\r\n");
      s_socket_state = SSTATE_RECEIVE_FAILED;
    }
  } break;

  default:
    break;
  }
}

static void resolve_cb(uint8_t *pu8DomainName, uint32_t u32ServerIP) {
  char buf[16];
  format_ipv4_addr(u32ServerIP, buf, sizeof(buf));
  printf("resolve_cb: %s => %s\r\n", pu8DomainName, buf);
  s_server_ip_addr = u32ServerIP;
}

static void install_callback_hooks() {
  if (s_orig_wifi_cb != gstrHifCxt.pfWifiCb) {
    s_orig_wifi_cb = gstrHifCxt.pfWifiC;
    hif_register_cb(M2M_REQ_GROUP_WIFI, my_wifi_cb);
  }
  if (s_orig_ip_cb != gstrHifCxt.pfIpCb) {
    s_orig_ip_cb = gstrHifCxt.pfIpC;
    hif_register_cb(M2M_REQ_GROUP_WIFI, my_wifi_cb);
  }
  hif_register_cb(M2M_REQ_GROUP_IP, my_ip_cb);
}

static void my_wifi_cb(uint8 u8OpCode, uint16 u16DataSize, uint32 u32Addr) {
  s_orig_wifi_cb(u8OpCode, u16DataSize, u32Addr);
  mu_sched_add(&s_sched_add, &s_tcp_task);
}

static void my_ip_cb(uint8 u8OpCode, uint16 u16BufferSize, uint32 u32Address) {
  s_orig_ip_cb(u8OpCode, u16BufferSize, u32Address);
  mu_sched_add(&s_sched_add, &s_tcp_task);
}
