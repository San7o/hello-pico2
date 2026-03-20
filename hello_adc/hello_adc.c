// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

//
// Hello ADC
// =========
//
// An Analog to digital Converter is an electronic integrated circuit
// used to convert analog signals such as voltages to digital or
// binary form consisting of 1s and 0s. The range of our ADC on Pico
// is 12 bits, which means the resolution is 2^12=4096, and it
// represents a range (at 3.3V) will be divided equally to 4096 parts.
//
// The conversion formula is as follows:
//
//           ADC Value = (Analog Voltage) / 3.3 * 1023
//
// Raspberry Pi Pico has four ADC channels, which are ADC0(GP26),
// ADC1(GP27), ADC2(GP28), ADC3(GP29). ADC3 used to measure VSYS on
// Pico board. Therefore, there are only three generic ADC channels
// that can be directly used, namely, ADC0, ADC1 and ADC2.
//
//
// Potentiometer
// -------------
//
// To play with ADC, we will use a potentiometer, which is essentially
// a variable resistor. It has 3 pins: pins 1 and 2 are connected to
// the positive and negative electrodes of power, while pic 3 will
// output the variable voltage within the range of the power supply.
//
// We read pin 3 with ADC0 and log it via USB.
//

#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <hardware/adc.h>

#define PIN_ANALOG_IN 26

void pico_init(void)
{
  stdio_init_all();

  adc_init();
  adc_gpio_init(PIN_ANALOG_IN);
  adc_select_input(0); // ADC0 (GP26)

  printf("Pico initialized\n");
  return;
}

int main(void)
{
  pico_init();
  while (true)
  {
    const float conversion_factor = 3.3f / (1 << 12);
    uint16_t result = adc_read();
    printf("Raw value: 0x%03x, voltage: %f V\n",
           result, result * conversion_factor);
    sleep_ms(500);
  }
  return 0;
}
