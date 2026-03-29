// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

//
// Rfid
// ====
//

#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/spi.h>

void pico_init(void)
{
  spi_init(spi0, );
  return;
}

int main(void)
{
  pico_init();
  
  while (true)
  {
    // TODO
  }
  return 0;
}
