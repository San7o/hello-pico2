// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

//
// Buttons pad
// ===========
//

#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>

#define LED_PIN 25
#define BUTTON_1_GPIO_PIN 16
#define BUTTON_2_GPIO_PIN 17
#define BUTTON_3_GPIO_PIN 18
#define BUTTON_4_GPIO_PIN 19

volatile int last_button_pressed = -1;

void button_callback(uint gpio, uint32_t event_mask)
{
  // Clear the interrupt
  gpio_acknowledge_irq(gpio, event_mask);
  
  last_button_pressed = gpio - BUTTON_1_GPIO_PIN;
  return;
}

void pico_init(void)
{
  stdio_init_all();

  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  
  gpio_init(BUTTON_1_GPIO_PIN);
  gpio_init(BUTTON_2_GPIO_PIN);
  gpio_init(BUTTON_3_GPIO_PIN);
  gpio_init(BUTTON_4_GPIO_PIN);

  gpio_set_dir(BUTTON_1_GPIO_PIN, GPIO_IN);
  gpio_set_dir(BUTTON_2_GPIO_PIN, GPIO_IN);
  gpio_set_dir(BUTTON_3_GPIO_PIN, GPIO_IN);
  gpio_set_dir(BUTTON_4_GPIO_PIN, GPIO_IN);

  gpio_pull_up(BUTTON_1_GPIO_PIN);
  gpio_pull_up(BUTTON_2_GPIO_PIN);
  gpio_pull_up(BUTTON_3_GPIO_PIN);
  gpio_pull_up(BUTTON_4_GPIO_PIN);
  
  gpio_set_irq_enabled_with_callback(BUTTON_1_GPIO_PIN, GPIO_IRQ_EDGE_FALL,
                                     true, button_callback);
  gpio_set_irq_enabled_with_callback(BUTTON_2_GPIO_PIN, GPIO_IRQ_EDGE_FALL,
                                     true, button_callback);
  gpio_set_irq_enabled_with_callback(BUTTON_3_GPIO_PIN, GPIO_IRQ_EDGE_FALL,
                                     true, button_callback);
  gpio_set_irq_enabled_with_callback(BUTTON_4_GPIO_PIN, GPIO_IRQ_EDGE_FALL,
                                     true, button_callback);
  
  return;
}

int main(void)
{
  pico_init();
  
  while (true)
  {
    if (last_button_pressed != -1)
    {
      printf("%d\n", last_button_pressed);
      // Use led pin as debug
      gpio_put(LED_PIN, !gpio_get(LED_PIN));
      
      last_button_pressed = -1;
    }
  }
  return 0;
}
