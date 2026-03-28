// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

//
// Audio
// =====
//
// We will be using a speaker connected to an audio converter and
// amplifier module. We talk to the module via the I2S interface,
// it is a serial interface protocol for transmitting two-channel
// digital audio as pule-code modulation, where the amplitude of the
// analog signal is sampled at uniform intervals and each sample is
// quantized to the nearest value within a range of digital steps.
//
// We use the following ports:
//
//    BCK: Audio data bit clock input
//    DIN: Audio data input
//    LCK: Audio data word clock input
//    VCC: 5V
//    GND
//
// We have two DMA buffers, we fill one while we move the other to
// the PIO. The PIO is a tiny processor that runs a very simple
// assembly-like script. Its job is to:
// - Wait for a 32-bit world to arrive from the DMA
// - Shift those bits out one by one on the DIN pin
// - Pulse the BCK for every bit
// - Flip the LCk once all 16 bits of a channel are sent
//
// We need to confiture the DMA to read from memory and write to the
// PIO's FIFO buffer when a DREQ is called, and swap buffers to
// prevent gaps in the music.
//
// 

#define BCK_GPIO_PIN 6
#define DIN_GPIO_PIN 8
#define LCK_GPIO_PIN 7

#include <stdio.h>
#include <pico/stdlib.h>

void pico_init(void)
{
  // TODO
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
