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

typedef struct ButtonEvent
{
  bool pressed;   // true = pressed, false = released
  uint key;       // 0 to 3
} ButtonEvent;

// TODO: Add an event buffer
volatile bool new_event = false;
volatile ButtonEvent event;

void button_callback(uint gpio, uint32_t event_mask)
{
  // Clear the interrupt
  gpio_acknowledge_irq(gpio, event_mask);

  event.pressed = event_mask & GPIO_IRQ_EDGE_RISE;
  event.key     = gpio - BUTTON_1_GPIO_PIN;
  new_event     = true;
  
  return;
}

void pico_init(void)
{
  stdio_init_all();

  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  uint buttons[] = {
    BUTTON_1_GPIO_PIN,
    BUTTON_2_GPIO_PIN,
    BUTTON_3_GPIO_PIN,
    BUTTON_4_GPIO_PIN,
  };

  for (int i = 0; i < 4; ++i)
  {
    gpio_init(buttons[i]);
    gpio_set_dir(buttons[i], GPIO_IN);
    gpio_pull_up(buttons[i]);
    gpio_set_irq_enabled_with_callback(buttons[i],
                                       GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE,
                                     true, button_callback);      
  }
  
  return;
}

int main(void)
{
  pico_init();
  
  while (true)
  {
    if (new_event)
    {
      printf("%d %d\n", event.key, event.pressed);
      // Use led pin as debug
      gpio_put(LED_PIN, !gpio_get(LED_PIN));
      
      new_event = false;
    }
  }
  return 0;
}
