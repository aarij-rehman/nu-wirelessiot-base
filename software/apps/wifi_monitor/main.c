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

// Pin configurations
#define UART_RX              NRF_GPIO_PIN_MAP(0, 8)
#define UART_TX              NRF_GPIO_PIN_MAP(0, 6)
#define UART_TX_BUF_SIZE     512
#define UART_RX_BUF_SIZE     512
#define MAX_TEST_DATA_BYTES     (15U)   

// Define
#define SSID "APT P2"
#define PWD "nestor123"

// Global Buffer
struct B
{
  char data[256]; 
  size_t size; 
} buf;
// error handler for UART
void uart_error_handle (app_uart_evt_t * p_event) {
  if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR) 
    {
    printf("communicatoin error \n");
    APP_ERROR_HANDLER(p_event->data.error_communication);
    } 
  else if (p_event->evt_type == APP_UART_FIFO_ERROR) 
    {
    printf("FIFO error \n");
    APP_ERROR_HANDLER(p_event->data.error_code);
    }
  else if (p_event->evt_type == APP_UART_DATA_READY) 
    {
      printf("data ready \n");
    }
  else if (p_event->evt_type == APP_UART_DATA) 
    {
      uint8_t ret;
      app_uart_get(&ret);
      char* ch = (char *) &ret;
      strcpy(&(buf.data[buf.size]), ch);
      printf("%c", buf.data[buf.size]);
      buf.size++;
    }
}

// initialization of UART
void uart_init(void) {
  uint32_t err_code;

  // This structure hold configuration information for the peripheral
  // configure RX and TX pins
  // no RTS or CTS pins with flow control disabled
  // no parity
  // baudrate 115200
  const app_uart_comm_params_t comm_params = {
    (uint32_t) 8, 
    (uint32_t) 6,
    0,
    0,
    APP_UART_FLOW_CONTROL_DISABLED,
    false,
    NRF_UARTE_BAUDRATE_115200
  };

  // actually initialize UART
  APP_UART_FIFO_INIT(
    &comm_params, 
    UART_RX_BUF_SIZE, 
    UART_TX_BUF_SIZE,
    uart_error_handle, 
    APP_IRQ_PRIORITY_LOW, 
    err_code);
  APP_ERROR_CHECK(err_code);

}

void esp_send(const char* command) {
  uint8_t len = strlen(command); 
  uint8_t* data = (uint8_t *) command;
  uint32_t i = 0;
  while (i < len) { 
    printf("sending %c \n", data[i]);
    while (app_uart_put(data[i]) != NRF_SUCCESS);

    i++;
  } 
}

// waits for a command to finish and prepares for the next one
int esp_wait() { 
  while (strstr((const char *) &buf.data, "OK\r\n") == NULL) {
    printf("waiting 1 second\n");
    printf("buffer: %s\n", buf.data);
    nrf_delay_ms(1000); // wait until you read "OK" in the response
  }
  // reset the buffer
  printf("resetting buffer");
  printf("old buffer %s", buf.data);
  buf.data[0] = 0;
  buf.size = 0;
  printf("new buffer %s", buf.data);
  return 0;
}

void esp_init() { 
  char connect[128];
  esp_send("AT+CWMODE=1\r\n"); // set ESP to client mode
  nrf_delay_ms(3000);
  
  sprintf(connect, "AT+CWJAP=\"%s\",\"%s\"\r\n",SSID, PWD); // connect to network
  esp_send(connect);
  nrf_delay_ms(10000);
}
// clear the buffer
void reset_buffer() {
  buf.size = 0;
  memset(buf.data, 0, sizeof(buf.data));
}
int esp_get_ping() { 
  int ret;
  esp_send("AT+PING=\"www.google.com\"\r\n");
  nrf_delay_ms(2000);

  sscanf(&(buf.data[buf.size-10]), "%d", &ret); // extract the ping
  reset_buffer()
  return ret;
}

int esp_get_rssi() { 
  int ret;
  esp_send("AT+CWJAP?\r\n");
  nrf_delay_ms(1000);
  sscanf(&(buf.data[buf.size-10]), "%d", &ret); // extract the rssi
  reset_buffer();
  return ret * (-1);
}

int main(void) {
  // init uart
  uart_init();
  esp_get_rssi();
  esp_get_ping();



  while (1) {
    nrf_delay_ms(1000);
  }
}

