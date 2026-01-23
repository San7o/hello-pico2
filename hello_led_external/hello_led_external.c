// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <stdio.h>
#include <pico/stdlib.h>

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 250
#endif

#define LED_PIN 15 

void pico_led_init(void)
{
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  return;
}

int main(void)
{
  pico_led_init();
  while (true)
  {
    gpio_put(LED_PIN, true);
    sleep_ms(LED_DELAY_MS);
    gpio_put(LED_PIN, false);
    sleep_ms(LED_DELAY_MS);
  }
  return 0;
}
