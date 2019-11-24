/**
 *
 * \file
 *
 * \brief WINC1500 TCP Client Example
 *
 * Relevant documents:
 *  SAM E54 Xplained Pro Users's Guide:
 * http://ww1.microchip.com/downloads/en/DeviceDoc/70005321A.pdf SAM E54
 * Xplained Pro Design Docs:
 * http://ww1.microchip.com/downloads/en/DeviceDoc/SAM_E54_Xplained_Pro_Design_Documentation.zip
 *  WINC 1500 Programming:
 * http://ww1.microchip.com/downloads/en/Appnotes/ATWINC15x0-ATWINC3400%20Wi-Fi-Network-Controller-Software-Programming-Guide-Application-Note-DS70005305C.pdf
 *  WINC 1500 Wi-Fi Network Controller Software Design Guide:
 * http://ww1.microchip.com/downloads/en/DeviceDoc/ATWINC1500-Wi-Fi-Network-Controller-Software-Design-Guide-User-Guide-DS00002389C.pdf
 *  Software Programming Guide:
 * http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-42639-Software-Programming-Guide-for-ATWINC3400-WiFi-using-SAMD21-Xplained-Pro_UserGuide.pdf
 *
 * Robert D Poor <rdpoor@klatunetworks.com>
 * October 2019
 */

#include "atmel_start.h"
#include "common/include/nm_common.h"
#include "driver/include/m2m_wifi.h"
#include "socket/include/socket.h"
#include "stdio_start.h"
#include "winc_init.h"
#include "mu_task.h"
#include "mu_port.h"
#include "mu_sched.h"
#include "mu_queue.h"
#include "tcp_client.h"
#include <string.h>

// =============================================================================
// local definitions and types

// REQUIRED: Edit the following liens to match your Wifi Access Point
#define MAIN_WLAN_SSID "pichincha"          /* AP SSID */
#define MAIN_WLAN_AUTH M2M_WIFI_SEC_WPA_PSK /* AP Security protocol */
#define MAIN_WLAN_PSK "robandmarisol"       /* AP password */


// #define USE_HTTPS

#ifdef USE_HTTPS
  #define SERVER_IP_PORT 443
  #define SERVER_NAME "api.traxxekg.com"
  #define SOCKET_FLAGS SOCKET_FLAGS_SSL
#else
  #define SERVER_IP_PORT 80
  #define SERVER_NAME "http://128.30.16.91"
  #define SOCKET_FLAGS 0
#endif

#define SERVER_REQUEST                                                         \
  "GET / HTTP/1.1\r\n"                                                         \
  "Host: lambda.ai.mit.edu\r\n"                                                \
  "User-Agent: Robert Poor <r@alum.mit.edu> is testing a tiny tcp client\r\n"  \
  "Accept: */*\r\n"                                                            \
  "\r\n"

#define MAIN_WIFI_M2M_BUFFER_SIZE 1460

// typedef struct {
//   uint32_t start;
//   uint32_t ncalls;
//   uint32_t t_accum;
//   uint32_t t_max;
// } state_telemetry_t;

// # of interrupts notifications we can handle between calls to sched_step().
// Must be a power of two.
#define ISR_QUEUE_POOL_SIZE 8

// =============================================================================
// local storage

// buffer for requests and responses
static char s_req_buffer[MAIN_WIFI_M2M_BUFFER_SIZE];
static char s_rsp_buffer[MAIN_WIFI_M2M_BUFFER_SIZE];

static mu_task_t s_response_task;

/**
 * \brief s_sched is an instance of the mulib scheduler.  You schedule an task
 * to trigger at some point in the future through a call to mu_sched_queue().
 */
static mu_sched_t s_sched;

/**
 * \brief Allocate storage for the ISR task queue.
 */
static mu_queue_obj_t s_isr_queue_pool[ISR_QUEUE_POOL_SIZE];

// =============================================================================
// local function declarations

static void response_task_fn(void *self, void *arg);

// =============================================================================
// public functions

/**
 * \brief Main application function.
 *
 * Initialize system, UART console, network then test function of TCP client.
 */
int main(void) {
	mu_string_t req_string;
	mu_string_t rsp_string;
	mu_task_t response_task;

  atmel_start_init();
  nm_bsp_init();
  mu_port_init();
  printf("tcp_client test\r\n");

  tcp_client_t *tcp_client = tcp_client_get_instance();

  // Initialize scheduler with storage for tasks queued from interrupt level.
  mu_sched_init(&s_sched, s_isr_queue_pool, ISR_QUEUE_POOL_SIZE);

  tcp_client_init(tcp_client, &s_sched, SERVER_NAME, 0);

  mu_task_init_immed(&s_response_task, response_task_fn, NULL, "Response Task");

  tcp_client_set_wlan_params(tcp_client,
                              MAIN_WLAN_SSID,
                              MAIN_WLAN_AUTH,
                              MAIN_WLAN_PSK,
                              M2M_WIFI_CH_ALL);

  mu_string_init(&req_string, s_req_buffer, MAIN_WIFI_M2M_BUFFER_SIZE);
  mu_string_sprintf(&req_string, "%s", SERVER_REQUEST);

  mu_string_init(&rsp_string, s_rsp_buffer, MAIN_WIFI_M2M_BUFFER_SIZE);

  tcp_client_request(tcp_client, &req_string, &rsp_string, &response_task);

  while (1) {
    mu_sched_step(&s_sched);
  }

  return 0;
}

// =============================================================================
// local functions

static void response_task_fn(void *self, void *arg) {
  tcp_client_t *tcp_client = (tcp_client_t *)arg;
  mu_string_t *response = tcp_client_get_response(tcp_client);
  printf("====\r\n%s\r\n====\r\n", mu_string_cstring(response));
}
