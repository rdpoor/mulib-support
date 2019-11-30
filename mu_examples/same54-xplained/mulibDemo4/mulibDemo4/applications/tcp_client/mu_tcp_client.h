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

#ifndef MU_TCP_CLIENT_H_
#define MU_TCP_CLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include <stdbool.h>
#include <stdint.h>
#include "mu_sched.h"
#include "mu_string.h"
#include "mu_task.h"
#include "mu_time.h"

// =============================================================================
// types and definitions

// This block defines the possible states for the process_transaction() state
// machine.
#define DEFINE_MU_TCP_CLIENT_STATES                         \
  DEFINE_MU_TCP_CLIENT_STATE(ENTRY)                         \
  DEFINE_MU_TCP_CLIENT_STATE(INITIATING_AP_CONNECTION)      \
  DEFINE_MU_TCP_CLIENT_STATE(AWAITING_AP_CONNECTION)        \
  DEFINE_MU_TCP_CLIENT_STATE(AP_CONNECT_FAILURE)            \
  DEFINE_MU_TCP_CLIENT_STATE(INITIATING_DNS_RESOLUTION)     \
  DEFINE_MU_TCP_CLIENT_STATE(AWAITING_DNS_RESOLUTION)       \
  DEFINE_MU_TCP_CLIENT_STATE(GETHOSTBYNAME_FAILURE)         \
  DEFINE_MU_TCP_CLIENT_STATE(DNS_RESOLUTION_FAILURE)        \
  DEFINE_MU_TCP_CLIENT_STATE(INITIATING_SOCKET_CONNECTION)  \
  DEFINE_MU_TCP_CLIENT_STATE(AWAITING_SOCKET_CONNECTION)    \
  DEFINE_MU_TCP_CLIENT_STATE(SOCKET_FAILURE)                \
  DEFINE_MU_TCP_CLIENT_STATE(CONNECT_FAILURE)               \
  DEFINE_MU_TCP_CLIENT_STATE(INITIATING_SEND)               \
  DEFINE_MU_TCP_CLIENT_STATE(AWAITING_SEND_CONFIRMATION)    \
  DEFINE_MU_TCP_CLIENT_STATE(SEND_FAILURE)                  \
  DEFINE_MU_TCP_CLIENT_STATE(INITIATING_RECEIVE)            \
  DEFINE_MU_TCP_CLIENT_STATE(AWAITING_RECEIVE_CONFIRMATION) \
  DEFINE_MU_TCP_CLIENT_STATE(RECEIVE_FAILURE)               \
  DEFINE_MU_TCP_CLIENT_STATE(SUCCESS)

// Expand DEFINE_MU_TCP_CLIENT_STATES to generate an enum for each state
#undef DEFINE_MU_TCP_CLIENT_STATE
#define DEFINE_MU_TCP_CLIENT_STATE(x) MU_TSTATE_##x,
typedef enum { DEFINE_MU_TCP_CLIENT_STATES } mu_tcp_client_state_t;
#undef DEFINE_MU_TCP_CLIENT_STATE

struct tcp_client_;  // defined in tcp_client.c

typedef struct tcp_client_ tcp_client_t;

typedef enum {
  TCP_CLIENT_ERR_NONE,
  TCP_CLIENT_ERR_BAD_FORMAT,
} tcp_client_err_t;

// =============================================================================
// declarations

/**
 * \brief Get an instance of an tcp_client object.
 *
 * tcp_client is implemented as a singleton object.
 *
 * \return An instance of an tcp_client object.
 */
tcp_client_t* tcp_client_get_instance();

/**
 * \brief Prepare for one transaction with an HTTP server.
 *
 * \param tcp_client An instance of tcp_client.
 * \param sched An instance of a mu_scheduler, required for asynchronous
 *        processing of events.
 * \param hostname May have the form [<protocol>://]<address>[:<port>], where
          address may be a hostname or V4 dotted IP address.
 * \param timeout If timeout is non-zero, a call to  `tcp_client_request()`
 *        will timeout after that many seconds.
 *
 * \return TCP_CLIENT_ERR_NONE on success, other values as appropriate.
 */
tcp_client_err_t tcp_client_init(tcp_client_t* tcp_client, mu_sched_t* sched,
                                 const char* hostname,
                                 mu_time_seconds_t timeout);

mu_sched_t* tcp_client_get_sched(tcp_client_t* tcp_client);
const char* tcp_client_get_hostname(tcp_client_t* tcp_client);
mu_time_seconds_t tcp_client_get_timeout(tcp_client_t* tcp_client);

tcp_client_err_t tcp_client_set_wlan_params(tcp_client_t* tcp_client,
                                            const char* ssid, uint8_t sec_type,
                                            const char* psk, uint16_t channel);

mu_task_t* tcp_client_get_response_task(tcp_client_t* tcp_client);
mu_string_t* tcp_client_get_request(tcp_client_t* tcp_client);
mu_string_t* tcp_client_get_response(tcp_client_t* tcp_client);
uint16_t tcp_client_get_port(tcp_client_t* tcp_client);
mu_string_t* tcp_client_get_protocol_string(tcp_client_t* tcp_client);
mu_string_t* tcp_client_get_hostname_string(tcp_client_t* tcp_client);
bool tcp_client_is_tsl(tcp_client_t* tcp_client);
uint16_t tcp_client_get_host_ip_address(tcp_client_t* tcp_client);

/**
 * \brief return the internal state of the tcp_client.
 *
 * \return The internal state of the tcp client.  This can be used by the
 *         response task to discover if the request was successful or not.
 */
mu_tcp_client_state_t tcp_client_get_state(tcp_client_t* tcp_client);

/**
 * \brief Return the tcp client's internal state as a descriptive string.
 *
 * \return A string describing the state, or "Unknown State" if state is not
 * valid.
 */
const char* tcp_client_get_state_name(tcp_client_t* tcp_client);

/**
 * \brief Send a request to an HTTP server and wait for response or timeout.
 *
 * Upon completion or timeout, tcp_client will call the response_task with
 * the `void *arg` parameter set to the tcp_client instance.
 *
 * \param tcp_client An instance of tcp_client.
 * \param request A mu_string containing the request.
 * \param response A mu_string to receive the response.
 * \param response_task A task to trigger upon receiving a response or timeout.
 *
 * \param
 */
tcp_client_err_t tcp_client_request(tcp_client_t* tcp_client,
                                    mu_string_t* request, mu_string_t* response,
                                    mu_task_t* response_task);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef MU_TCP_CLIENT_H_ */
