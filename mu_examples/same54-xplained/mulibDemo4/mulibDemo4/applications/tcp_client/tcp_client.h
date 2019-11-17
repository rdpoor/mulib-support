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

#ifndef TCP_CLIENT_H_
#define TCP_CLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include "mu_sched.h"
#include "mu_string.h"
#include "mu_task.h"
#include "mu_time.h"
#include <stdbool.h>
#include <stdint.h>

// =============================================================================
// types and definitions

// This block defines the possible states for the process_transaction() state
// machine.  (`TCS` stands for HTTP Client State).
#define DEFINE_TCS_STATES                                                      \
  DEFINE_TCS_STATE(TCS_ENTRY)                                                  \
  DEFINE_TCS_STATE(TCS_INITIATING_AP_CONNECTION)                               \
  DEFINE_TCS_STATE(TCS_AWAITING_AP_CONNECTION)                                 \
  DEFINE_TCS_STATE(TCS_AP_CONNECT_FAILURE)                                     \
  DEFINE_TCS_STATE(TCS_INITIATING_DNS_RESOLUTION)                              \
  DEFINE_TCS_STATE(TCS_AWAITING_DNS_RESOLUTION)                                \
  DEFINE_TCS_STATE(TCS_GETHOSTBYNAME_FAILURE)                                  \
  DEFINE_TCS_STATE(TCS_DNS_RESOLUTION_FAILURE)                                 \
  DEFINE_TCS_STATE(TCS_INITIATING_SOCKET_CONNECTION)                           \
  DEFINE_TCS_STATE(TCS_AWAITING_SOCKET_CONNECTION)                             \
  DEFINE_TCS_STATE(TCS_SOCKET_FAILURE)                                         \
  DEFINE_TCS_STATE(TCS_CONNECT_FAILURE)                                        \
  DEFINE_TCS_STATE(TCS_INITIATING_SEND)                                        \
  DEFINE_TCS_STATE(TCS_AWAITING_SEND_CONFIRMATION)                             \
  DEFINE_TCS_STATE(TCS_SEND_FAILURE)                                           \
  DEFINE_TCS_STATE(TCS_INITIATING_RECEIVE)                                     \
  DEFINE_TCS_STATE(TCS_AWAITING_RECEIVE_CONFIRMATION)                          \
  DEFINE_TCS_STATE(TCS_RECEIVE_FAILURE)                                        \
  DEFINE_TCS_STATE(TCS_SUCCESS)

// Expand DEFINE_TCS_STATES to generate an enum for each state
#undef DEFINE_TCS_STATE
#define DEFINE_TCS_STATE(x) x,
typedef enum { DEFINE_TCS_STATES } hcs_t;

#undef DEFINE_TCS_STATE

struct tcp_client_; // defined in tcp_client.c

typedef struct tcp_client_ tcp_client_t;

typedef enum {
  TCP_CLIENT_ERR_NONE,
  TCP_CLIENT_ERR_BAD_FORMAT,
} tcp_client_err_t;

#define DEFINE_HTTP_METHODS                                                    \
  DEFINE_HTTP_METHOD(OPTIONS)                                                  \
  DEFINE_HTTP_METHOD(GET)                                                      \
  DEFINE_HTTP_METHOD(HEAD)                                                     \
  DEFINE_HTTP_METHOD(POST)                                                     \
  DEFINE_HTTP_METHOD(PUT)                                                      \
  DEFINE_HTTP_METHOD(DELETE)                                                   \
  DEFINE_HTTP_METHOD(TRACE)                                                    \
  DEFINE_HTTP_METHOD(CONNECT)

// Expand DEFINE_HTTP_METHODS to generate an enum for each method
#undef DEFINE_HTTP_METHOD
#define DEFINE_HTTP_METHOD(x) HTTP_METHOD_##x,
typedef enum { DEFINE_HTTP_METHODS } http_method_t;
#undef DEFINE_TCS_STATE

// =============================================================================
// declarations

/**
 * \brief Get an instance of an tcp_client object.
 *
 * tcp_client is implemented as a singleton object.
 *
 * \return An instance of an tcp_client object.
 */
tcp_client_t *tcp_client_get_instance();

/**
 * \brief Prepare for one transaction with an HTTP server.
 *
 * \param tcp_client An instance of tcp_client.
 * \param sched An instance of a mu_scheduler, required for asynchronous
 *        processing of events.
 * \param hostname
 * \param timeout If timeout is non-zero, a call to  `tcp_client_request()`
 *        will timeout affter that many seconds.
 *
 * \return TCP_CLIENT_ERR_NONE on success, other values as appropriate.
 */
tcp_client_err_t tcp_client_init(tcp_client_t *tcp_client,
mu_sched_t *sched,
const char *hostname,
mu_time_seconds_t timeout);

mu_sched_t *tcp_client_get_sched(tcp_client_t *tcp_client);
const char *tcp_client_get_hostname(tcp_client_t *tcp_client);
mu_time_seconds_t tcp_client_get_timeout(tcp_client_t *tcp_client);

tcp_client_err_t tcp_client_set_wlan_params(tcp_client_t *tcp_client,
                                              const char *ssid,
                                              uint8_t sec_type,
                                              const char *psk,
                                              uint16_t channel);

mu_task_t *tcp_client_get_response_task(tcp_client_t *tcp_client);
mu_string_t *tcp_client_get_request(tcp_client_t *tcp_client);
mu_string_t *tcp_client_get_response(tcp_client_t *tcp_client);
uint16_t tcp_client_get_port(tcp_client_t *tcp_client);
mu_string_t *tcp_client_get_protocol_string(tcp_client_t *tcp_client);
mu_string_t *tcp_client_get_hostname_string(tcp_client_t *tcp_client);
bool tcp_client_is_tsl(tcp_client_t *tcp_client);
uint16_t tcp_client_get_host_ip_address(tcp_client_t *tcp_client);

/**
 * \brief return the internal state of the tcp_client.
 *
 * \return The internal state of the tcp client.  This can be used by the
 *         response task to discover if the request was successful or not.
 */
hcs_t tcp_client_get_state(tcp_client_t *tcp_client);

/**
 * \brief Return the tcp client's internal state as a descriptive string.
 *
 * \return A string describing the state, or "Unknown if state is not valid.
 */
const char *tcp_client_get_state_name(tcp_client_t *tcp_client);

/**
 * \brief Send a request to an HTTP server and wait for response or timeout.
 *
 * Upon completion or timeout, tcp_client will call the response_task with
 * the `void *arg` parameter set to the tcp_client instance.  The response_task
 * can use this to access the response status and headers and body.
 *
 * \param tcp_client An instance of tcp_client.
 * \param method One of the HTTP method verbs: TCP_CLIENT_GET,
 *        TCP_CLIENT_POST, etc. \param url The URL for the request
 * \param response_task A task to trigger upon recieving a response or timeout.
 * \param req_headers A mu_string containing headers for the request.  May be
 *        null.
 * \param req_body A mu_string containing the body of the request.  May be null.
 * \param rsp_headers A mu_string to receive the response headers. May be
 *        null.
 * \param rsp_body A mu_string to receive the response body.  May be null.
 *
 * \param
 */
tcp_client_err_t tcp_client_request(tcp_client_t *tcp_client,
mu_string_t *request,
mu_string_t *response,
mu_task_t *response_task);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef MU_HTTP_TASK_H_ */
