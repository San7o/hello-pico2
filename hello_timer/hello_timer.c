// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

//
// Hello timer
// ===========
//

#include <stdio.h>
#include <pico/stdlib.h>

repeating_timer_t timer;

bool repearing_timer_callback(__unused repeating_timer_t *rt)
{
  printf("Fired timer\n");
  return true;
}

void pico_init(void)
{
  stdio_init_all();

  int32_t delay_ms = 500;
  add_repeating_timer_ms(delay_ms, repearing_timer_callback, NULL, &timer);
  
  return;
}

int main(void)
{
  pico_init();
  
  while (true) {}
  
  return 0;
}
