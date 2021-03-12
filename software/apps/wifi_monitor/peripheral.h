#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "app_uart.h"
#include "nrf_uarte.h"
#include "nrf_power.h"
#include "simple_ble.h"
#include "nrf52840dk.h"

#define SSID "ATT6yzKXPi"
#define PWD "jjtn%g8n7zp4"

struct B
{
  char data[256]; 
  size_t size; 
} buf;

extern bool service_request;

// Pin configurations
#define UART_RX              NRF_GPIO_PIN_MAP(0, 8)
#define UART_TX              NRF_GPIO_PIN_MAP(0, 6)
#define UART_TX_BUF_SIZE     512
#define UART_RX_BUF_SIZE     512
#define MAX_TEST_DATA_BYTES     (15U)   

// BLUETOOTH ADVERTISING SETUP
// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
        // c0:98:e5:4e:xx:xx
        .platform_id       = 0x4E,   // used as 4th octect in device BLE address
        .device_id         = 0xAABB, // must be unique on each device you program!
        .adv_name          = "CB497", // used in advertisements if there is room
        .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
        .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
        .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
};

simple_ble_app_t* simple_ble_app;

/* BLE FUNCTIONS */

/* Begin advertising the results of a speed test */
void begin_advertising(int ping, int rssi);

/* Callback handler for reading from central when to begin speedtest */
void ble_evt_adv_report(ble_evt_t const* p_ble_evt);

/* Begin scanning for central for when it's asking for data */
void begin_scanning();

/* END OF BLE FUNCTIONS */



/* WIFI FUNCTIONS */

// Function for connecting the ESP to the internet
void esp_init();

// Function for getting the ping from the network. Returns the ping.
int esp_get_ping();

// Function for getting the RSSI from the network. Returns the RSSI.
int esp_get_rssi();

// Function for running a speed test 
void esp_get_speed_test();

// Function to send commands to the ESP
void esp_send(const char* command); 

// Function which receives a message from the ESP and puts it 
// on the buffer 
void esp_receive(char* buf);

// Resets the global variable buffer (buf).
void reset_buffer();

/* END OF WIFI FUNCTIONS */



/* UART FUNCTIONS */

/* Handles events and errors on the UART module */
void uart_error_handle(app_uart_evt_t * p_event);

/* Initializes the UART module */
void uart_init();

/* END OF UART FUNCTIONS */
