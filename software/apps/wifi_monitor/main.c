#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "app_uart.h"
#include "nrf_uarte.h"
#include "nrf_power.h"

// Pin configurations
#define LED NRF_GPIO_PIN_MAP(0,13)
#define UART_RX              NRF_GPIO_PIN_MAP(0, 8)
#define UART_TX              NRF_GPIO_PIN_MAP(0, 6)
#define UART_TX_BUF_SIZE     256
#define UART_RX_BUF_SIZE     256
#define MAX_TEST_DATA_BYTES     (15U)   

// error handler for UART
void uart_error_handle (app_uart_evt_t * p_event) {
  uint8_t ret;
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
  else if (p_event->evt_type == APP_UART_TX_EMPTY) 
    {
      //printf("TX empty \n");
    }
  else if (p_event->evt_type == APP_UART_DATA) 
    {
      app_uart_get(&ret);
      printf("%c", ret);
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

int main(void) {

  // init led
  nrf_gpio_cfg_output(LED);
  nrf_gpio_pin_set(LED);

  // init uart
  uart_init();
  char* arg =  "AT+CWMODE=2\r\n";
  uint8_t* data = (uint8_t *) arg;

  // Send data
  nrf_delay_ms(1000);
  uint32_t i = 0;
  while (i < strlen(arg)) { 
    printf("sending %c\n", data[i]);
    while (app_uart_put(data[i]) != NRF_SUCCESS);
    i++;
  } 
  printf("Response: ");
  while (1) {
    nrf_delay_ms(1000);
  }
}

