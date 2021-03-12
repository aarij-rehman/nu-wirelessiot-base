#include "peripheral.h"
#include <stdlib.h>

int main(void) {
  // init uart
  uart_init();

  simple_ble_app = simple_ble_init(&ble_config);
  begin_scanning();
  advertising_stop();
  while (1) {
    printf("Currently scanning\n");
    if (service_request){
      int rssi = esp_get_rssi();
      int ping = esp_get_ping();
      printf("RSSI: %i\n", rssi);
      printf("PING: %i\n", ping);
      fflush(stdout);

      begin_advertising(ping, rssi);
      nrf_delay_ms(5000);
      advertising_stop();
      printf("STOPPED BLE ADVERTISING\n");
      service_request = false;
    }
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    nrf_delay_ms(2000);
  }
}