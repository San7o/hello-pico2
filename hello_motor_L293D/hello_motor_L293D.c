// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

//
// Hello motor L293D
// =================
//
// Power a motor via a potentiometer and a L293D.
//

#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/adc.h>
#include <hardware/pwm.h>

#define IN1_GPIO_PIN     15
#define IN2_GPIO_PIN     16
#define ENABLE1_GPIO_PIN 17
#define ADC0_GPIO_PIN    26

volatile bool rotation_dir;
volatile int rotation_speed;

void pico_init(void)
{
  stdio_init_all();
  
  gpio_init(IN1_GPIO_PIN);
  gpio_set_dir(IN1_GPIO_PIN, GPIO_OUT);
  
  gpio_init(IN2_GPIO_PIN);
  gpio_set_dir(IN2_GPIO_PIN, GPIO_OUT);
  
  gpio_init(ENABLE1_GPIO_PIN);
  gpio_set_dir(ENABLE1_GPIO_PIN, GPIO_OUT);

  adc_init();
  adc_gpio_init(ADC0_GPIO_PIN);
  adc_select_input(0); // ADC0 (GP26)
}

void analog_write(uint gpio, uint value)
{
  gpio_set_function(gpio, GPIO_FUNC_PWM);
  uint slice_num = pwm_gpio_to_slice_num(gpio);
  uint chan      = pwm_gpio_to_channel(gpio);
  pwm_set_wrap(slice_num, 255);
  pwm_set_chan_level(slice_num, chan, value);
  pwm_set_enabled(slice_num, true);
}

void drive_motor(bool dir, int spd)
{
  gpio_put(IN1_GPIO_PIN, dir);
  gpio_put(IN2_GPIO_PIN, !dir);
  analog_write(ENABLE1_GPIO_PIN, spd);
}

static inline int clamp(int x, int min, int max)
{
  if (x < min)
    return min;
  if (x > max)
    return max;
  return x;
}

static inline int abs(int x)
{
  return (x > 0) ? x : -x;
}

int main(void)
{
  pico_init();
  while (true)
  {
    uint16_t potentiometer_val = adc_read();
    rotation_speed = potentiometer_val - 2048;
    if (potentiometer_val > 2048)
      rotation_dir = true;
    else
      rotation_dir = false;

    int scaled_speed = (abs(rotation_speed) * 255) - 2048;
    drive_motor(rotation_dir, clamp(scaled_speed, 0, 255));

    printf("potentiometer_val: %u, rotation_dir: %d, rotation_speed: %d\n",
           potentiometer_val, rotation_dir, rotation_speed);

    sleep_ms(10);
  }
  return 0;
}
