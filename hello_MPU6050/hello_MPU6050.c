// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

//
// Hello MPU6050
// =============
//
// We use I2C to communicate with the module.
//

#include <stdio.h>
#include <string.h>
#include <pico/stdlib.h>
#include <pico/binary_info.h>
#include <hardware/i2c.h>

static int addr = 0x68;

#ifdef i2c_default

static void mpu6050_reset()
{
  uint8_t buf[] = {0x6B, 0x80};
  i2c_write_blocking(i2c_default, addr, buf, 2, false);
  sleep_ms(100);

  buf[1] = 0x00;
  i2c_write_blocking(i2c_default, addr, buf, 2, false);
  sleep_ms(10);
}

static void mpu6050_read_raw(uint16_t accel[3], int16_t gyro[3], int16_t *temp)
{
  uint8_t buffer[6];

  uint8_t val = 0x3B;
  i2c_write_blocking(i2c_default, addr, &val, 1, true);
  i2c_read_blocking(i2c_default, addr, buffer, 6, false);

  for (int i = 0; i < 3; ++i)
  {
    accel[i] = (buffer[i*2] << 8 | buffer[(i * 2) + 1]);
  }

  val = 0x43;
  i2c_write_blocking(i2c_default, addr, &val, 1, true);
  i2c_read_blocking(i2c_default, addr, buffer, 6, false);

  for (int i = 0; i < 3; ++i)
  {
    gyro[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
  }

  val = 0x41;
  i2c_write_blocking(i2c_default, addr, &val, 1, true);
  i2c_read_blocking(i2c_default, addr, buffer, 2, false);

  *temp = buffer[0] << 8 | buffer[1];
}

#endif // i2c_default

int main(void)
{
  stdio_init_all();

#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
#warning hello_MPU6050 example required a board with I2C pins
  puts("Default I2C pins were not defined");
  return 0;
  #else

  i2c_init(i2c_default, 400 * 1000);
  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
  bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN,
                             PICO_DEFAULT_I2C_SCL_PIN,
                             GPIO_FUNC_I2C));

  mpu6050_reset();

  int16_t acceleration[3], gyro[3], temp;

  while (1)
  {
    mpu6050_read_raw(acceleration, gyro, &temp);

    printf("Acc. X = %d, Y = %d, Z = %d\n",
           acceleration[0], acceleration[1], acceleration[2]);
    printf("Gyro. X = %d, Y = %d, Z = %d\n",
           gyro[0], gyro[1], gyro[2]);
    printf("Temp. = %f\n", (temp / 340.0) + 36.53);

    sleep_ms(100);
  }
  
  return 0;
  #endif
}
