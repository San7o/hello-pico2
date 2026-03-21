// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

//
// Servo control
// =============
//
// Control a servo motor via a potentiometer
//

#define PARAM_ASSERTIONS_ENABLED_PWM 1
#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/assert.h>
#include <hardware/pwm.h>
#include <hardware/clocks.h>
#include <hardware/gpio.h>
#include <hardware/adc.h>

#define SERVO_GPIO_PIN  16
#define ANALOG_GPIO_PIN 26

typedef struct
{
  uint gpio;
  uint slice;
  uint chan;
  uint speed;
  uint resolution;
  bool on;
  bool invert;
} Servo;

uint32_t pwm_set_freq_duty(uint slice_num, uint chan,
                           uint32_t f, int d)
{
  uint32_t clock = clock_get_hz(clk_sys);

  // Calculate the smallest possible clock divider that won't overflow
  // the PWM counter (16 bits on the Pico2).
  //
  // Given that the wrap needs to fit in 16 bits:
  //
  //      Wrap       = clock / (freq * divider) < (1 << 16)
  //
  // Then:
  //
  //       Divider   >= clock / (freq * (1<<16))
  //
  // Since the divider (and the wrap) is an integer number number, we
  // would lose fractional precision. To get more precision, multiply
  // this by 16 (to get the true divider, then divide it by 16):
  //
  //       Divider16 >= clock * 16 / (freq * (1<<16))
  //
  // (clock % (f * 4096) != 0) rounds up the divider. We do this
  // because if the divider is too small, then the wrap could be
  // too big and exceed 16 bits.
  //
  uint32_t divider16 = (clock / f / 4096) + (clock % (f * 4096) != 0);

  if (divider16 / 16 == 0)
    divider16 = 16;

  // The PWM counter starts at 0 and counts up to the wrap value. When
  // it hits the wrap, it resets to 0.  This defines the period (and
  // therefore the Frequency) of the signal. We want to set this to
  // `f` Hz.
  // We use the `Wrap` formula from above.
  uint32_t wrap = clock * 16 / divider16 / f - 1;
  pwm_set_clkdiv_int_frac(slice_num, divider16 / 16, divider16 & 0xF);
  pwm_set_wrap(slice_num, wrap);
  pwm_set_chan_level(slice_num, chan, wrap * d / 100);
  return wrap;
}

uint32_t pwm_get_wrap(uint slice_num)
{
  valid_params_if(PWM, slice_num >= 0 &&
                       slice_num < NUM_PWM_SLICES);
  return pwm_hw->slice[slice_num].top;
}

// Set position with an high resolution (0-10000)
void pwm_set_dutyH(uint slice_num, uint chan, int d)
{
  pwm_set_chan_level(slice_num, chan, pwm_get_wrap(slice_num) * d / 10000);
}

// Set position in terms of percentages
void servo_position(Servo *s, uint p)
{
  // min = 250 (0.5ms pulse), max = 1250 (2.5ms pulse)
  pwm_set_dutyH(s->slice, s->chan, p * 10 + 250);
}

void servo_on(Servo *s)
{
  pwm_set_enabled(s->slice, true);
  s->on = true;
}

void servo_off(Servo *s)
{
  pwm_set_enabled(s->slice, false);
  s->on = false;
}

void servo_init(Servo *s, uint gpio, bool invert)
{
  gpio_set_function(gpio, GPIO_FUNC_PWM);
  s->gpio = gpio;
  
  // Pins are grouped into slices (0-11) and channels (A and B), so we
  // store them here
  s->slice = pwm_gpio_to_slice_num(gpio);
  s->chan = pwm_gpio_to_channel(gpio);
  
  pwm_set_enabled(s->slice, false);
  s->on = false;
  s->speed = 0;
  // Locks the hardware into a 50hz rhythm
  s->resolution = pwm_set_freq_duty(s->slice,
                                    s->chan,
                                    50, // Hz
                                    0);

  // Set position to starting position
  pwm_set_dutyH(s->slice, s->chan, 250);

  // Set polarity based on the channel
  if (s->chan) // channel B
  {
    pwm_set_output_polarity(s->slice, false, invert);
  }
  else // channel A
  {
    pwm_set_output_polarity(s->slice, invert, false);
  }
  s->invert = invert;
}

void pico_init(void)
{
  adc_init();
  adc_gpio_init(ANALOG_GPIO_PIN);
  adc_select_input(0); // ADC0 (GP26)
}

int main(void)
{
  pico_init();
  
  Servo s1;
  servo_init(&s1, SERVO_GPIO_PIN, false);
  servo_on(&s1);
  
  while (true)
  {
    sleep_ms(50);
    uint16_t result = adc_read();
    servo_position(&s1, result * 100 / (1<<12));
  }
  return 0;
}
