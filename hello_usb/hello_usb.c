// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

//
// Hello usb
// =========
//
// Print something throught the USB. Yes, you can use printf.
// 
// Read output in:
//    sudo cat /dev/ttyACM0
//

#include <stdio.h>
#include <pico/stdlib.h>

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
