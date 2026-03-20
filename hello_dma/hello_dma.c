// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

//
// Hello DMA
// =========
//
// Direct Memory Access is a feature that allows hardware components
// to access the main system memory independently of the CPU. This
// enables faster data transfers and reduces CPU's workload.  A basic
// DMA engine takes a source address, a destination address and a
// length and copies data from source to destination.
//
// The Pico has 12 channels, allowing 12 copy operations at once.
//

#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/dma.h>

const char src[] = "Hello, world! (from DMA)";
char dst[count_of(src)];

void pico_init(void)
{
  stdio_init_all();

  return;
}

int main(void)
{
  pico_init();

  // Get a free channel, panic() if there are none
  int chan = dma_claim_unused_channel(true);

  // 8 bit transfoes. Both read and write address increment after each
  // transfer (each pointing to a location in src or dst
  // respectively). No DREQ is selected, so the DMA transfers as fast
  // as it can.

  dma_channel_config c = dma_channel_get_default_config(chan);
  channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
  channel_config_set_read_increment(&c, true);
  channel_config_set_write_increment(&c, true);

  dma_channel_configure(
                        chan, // Channel to be configured
                        &c,   // Configuration
                        dst,  // The initial write address
                        src,  // The initial read address
                        count_of(src), // Number of transfers (1 byte each)
                        true  // Start immediately
                        );

  dma_channel_wait_for_finish_blocking(chan);

  while(true)
  {
    sleep_ms(1000);
    puts(dst);
  }
  
  return 0;
}
