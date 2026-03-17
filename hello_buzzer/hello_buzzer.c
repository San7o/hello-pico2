// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

//
// Hello buzzer
// ============
//
// Play a buzzer when a button is pressed.
//

#include <pico/stdlib.h>

#define PIN_BUZZER 15
#define PIN_BUTTON 16

void pico_init(void)
{
  gpio_init(PIN_BUTTON);
  gpio_set_dir(PIN_BUTTON, GPIO_IN);
  
  gpio_init(PIN_BUZZER);
  gpio_set_dir(PIN_BUZZER, GPIO_OUT);

  gpio_put(PIN_BUZZER, false);  
  return;
}

void loop(void)
{
  if (gpio_get(PIN_BUTTON) == false)
  {
    gpio_put(PIN_BUZZER, true);
  }
  else
  {
    gpio_put(PIN_BUZZER, false);    
  }
}

int main(void)
{
  pico_init();

  while (true)
    loop();
  
  return 0;
}
