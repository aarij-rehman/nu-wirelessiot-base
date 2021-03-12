#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "nrf.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_power.h"
#include "nrf52840dk.h"
#include "simple_ble.h"
// Must be able to request individual metrics
// Configure buttion IO to get metrics
// Advertise Bluetooth
// Peripherals listening for Central
// When it hears command, run tests
// Put everything in manufacture data

static simple_ble_config_t ble_config = {
        // c0:98:e5:4e:xx:xx
        .platform_id       = 0x4E,   // used as 4th octect in device BLE address
        .device_id         = 0xAABD, // must be unique on each device you program!
        .adv_name          = "Central", // used in advertisements if there is room
        .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
        .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
        .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
};
simple_ble_app_t* simple_ble_app;

// Callback handler for advertisement reception

/* SCANNER DEFINITIONS */
void ble_evt_adv_report(ble_evt_t const* p_ble_evt);
/* END OF SCANNER DEFINITIONS */

/* Configures button to renew advertising() */
void configure_button_renew_advertising();

/* Begin advertising to tell peripheral to renew data */
void begin_advertising();

/* Begin scanning to read peripheral data payload*/
void begin_scanning();

/* Reads a particular metric of received payload data */
void request_metric_advertising();

/* Reads all the data stored in advertising data of BLE */
void read_peripheral_advertising();
