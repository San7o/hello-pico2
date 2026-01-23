// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <stdio.h>
#include <pico/stdlib.h>

//
// Print something throught the USB
// 
// Read output in:
//    sudo cat /dev/ttyACM0
//

int main(void)
{
  stdio_init_all();
  while (true)
  {
    printf("Hello, world!\n");
    sleep_ms(1000);
  }
  return 0;
}
