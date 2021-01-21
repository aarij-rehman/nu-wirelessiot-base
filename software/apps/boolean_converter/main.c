// Buttons app
//
// Use buttons and switches to control LEDs
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_gpiote.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"

#include "nrf52840dk.h"

int main(void) {

  // Initialize.
  nrf_gpio_cfg_output(LED1);
  nrf_gpio_cfg_output(LED2);
  nrf_gpio_cfg_output(LED3);
  nrf_gpio_cfg_output(LED4);
  nrf_gpio_pin_set(LED1);
  nrf_gpio_pin_set(LED2);
  nrf_gpio_pin_set(LED3);
  nrf_gpio_pin_set(LED4);
  nrf_gpio_cfg_input(BUTTON1, NRF_GPIO_PIN_PULLUP);
  nrf_gpio_cfg_input(BUTTON2, NRF_GPIO_PIN_PULLUP);
  nrf_gpio_cfg_input(BUTTON3, NRF_GPIO_PIN_PULLUP);
  nrf_gpio_cfg_input(BUTTON4, NRF_GPIO_PIN_PULLUP);

  // Enter main loop.
  
  // Stuff copied over from printf to initialize printing
  ret_code_t error_code = NRF_SUCCESS;

  nrf_gpio_cfg_output(LED1);

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Boolean Calculator App: \n");
  printf("Value in Binary: ");

  // loop foreve
  int sum = 0;
  while (1) {
    if (nrf_gpio_pin_read(BUTTON1)) {
      nrf_gpio_pin_set(LED1);
    } else {
      nrf_gpio_pin_clear(LED1);
      printf("0");
      sum = sum * 2;
      nrf_delay_ms(250);
    }

    if (nrf_gpio_pin_read(BUTTON2)) {
      nrf_gpio_pin_set(LED2);
    } else {
      nrf_gpio_pin_clear(LED2);
      printf("1");
      sum = sum*2 + 1;
      nrf_delay_ms(250);
    }


    if (nrf_gpio_pin_read(BUTTON4)) {
      nrf_gpio_pin_set(LED4);
    } else {
      nrf_gpio_pin_clear(LED4);
      printf("\nValue in decimal: %i", sum);
      nrf_delay_ms(1000);
      sum = 0;
      printf("\n\nValue in Binary: ");
      nrf_delay_ms(500);
    }
  }
}
