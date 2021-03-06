// BLE Scanning app
//
// Receives BLE advertisements

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "simple_ble.h"

#include "nrf52840dk.h"

// BLE configuration
// This is mostly irrelevant since we are scanning only
static simple_ble_config_t ble_config = {
        // BLE address is c0:98:e5:4e:00:02
        .platform_id       = 0x4E,    // used as 4th octet in device BLE address
        .device_id         = 0x0002,  // used as the 5th and 6th octet in the device BLE address, you will need to change this for each device you have
        .adv_name          = "CS397/497", // irrelevant in this example
        .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS), // send a packet once per second (minimum is 20 ms)
        .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS), // irrelevant if advertising only
        .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS), // irrelevant if advertising only
};
simple_ble_app_t* simple_ble_app;


// Callback handler for advertisement reception
void ble_evt_adv_report(ble_evt_t const* p_ble_evt) {

  // extract the fields we care about
  ble_gap_evt_adv_report_t const* adv_report = &(p_ble_evt->evt.gap_evt.params.adv_report);
  uint8_t const* ble_addr = adv_report->peer_addr.addr; // array of 6 bytes of the address
  uint8_t* adv_buf = adv_report->data.p_data; // array of up to 31 bytes of advertisement payload data
  uint16_t adv_len = adv_report->data.len; // length of advertisement payload data

  // Get mac address in big endian aka readable format
  uint8_t mac_address[6] = {ble_addr[5], ble_addr[4], ble_addr[3], ble_addr[2], ble_addr[1], ble_addr[0]};
  
  // filter to only read the board's data
  if (mac_address[0] == 0xc0 && mac_address[1] == 0x98){
    printf("\n\nRECEIVED AN ADVERTISEMENT!\n");
    printf("MAC Address: %x:%x:%x:%x:%x:%x\n", mac_address[0], mac_address[1], mac_address[2], mac_address[3], mac_address[4], mac_address[5]);
    printf("Payload Length: %d\n", adv_len);
    
    // print the contents relating to flags
    // first byte is len of flags, second byte is telling you it's flags, third byte is the flags
    printf("Flag Settings: 0x%02x\n", adv_buf[2]);

    // parse info
    int curr = 3;
    int end;
    while (curr<adv_len) { 
      end = curr + adv_buf[curr] + 1;
      printf("Lenght of Data: %i\n", adv_buf[curr]);
      curr++;
      printf("Type of Data: 0x%02x\n", adv_buf[curr]);
      curr++;
      printf("Data: ");
      while (curr < end) { 
        printf("%c", adv_buf[curr]);
        curr++;
        }
      }
  }
}


int main(void) {

  // Setup BLE
  // Note: simple BLE is our own library. You can find it in `nrf5x-base/lib/simple_ble/`
  simple_ble_app = simple_ble_init(&ble_config);
  advertising_stop();

  // Start scanning
  scanning_start();

  // go into low power mode
  while(1) {
    power_manage();
  }
}



