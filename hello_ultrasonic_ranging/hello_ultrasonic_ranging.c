// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

//
// Ultrasonic Range Pins
// ---------------------
//
//   VCC   Power supply pin (needs 5V)
//   Trig  Trigger pin
//   Echo  Echo pin
//   GND   Ground
//
// Operation
// ---------
//
// To transmit an ultrasonic wave, you output a high-level pulse in
// `Trig` pin lasting for least 10us. At the same time, the `Echo` pin
// is pulled up. When the module receives the returned ultrasonic
// waves from encountering an obstacle, the `Echo` pin will be pulled
// down. The duration of high level in the Echo pin is the total time
// of the ultrasonic wave from transmitting to receiving, s=vt/2.
//
// You can read the output with:
//
//    sudo cat /dev/ttyACM0
//

#include <stdio.h>
#include <pico/stdlib.h>

#define TRIGGER_PIN     19
#define ECHO_PIN        18
#define SOUND_VELOCITY  340   // m/s

#define MAX_DISTANCE    700   // cm
#define TIMEOUT_US      (2 * MAX_DISTANCE / 100 / SOUND_VELOCITY * 1000000)

static volatile uint64_t ping_start_time = 0;
static volatile float    last_distance_cm = 0.0f;

// This callback is called at rise or fall of the `Echo` pin. It sets
// the global `delta_time` variable with the time in microseconds
// between rise and fall.
void echo_callback(uint gpio, uint32_t event_mask)
{
  if (event_mask & GPIO_IRQ_EDGE_RISE)
  {
    ping_start_time = to_us_since_boot(get_absolute_time());
    return;
  }
  else if (event_mask & GPIO_IRQ_EDGE_FALL)
  {
    if (ping_start_time == 0)
      return;
    
    uint64_t ping_end_time = to_us_since_boot(get_absolute_time());
    uint64_t delta_time    = ping_end_time - ping_start_time;
  
    last_distance_cm = (float) delta_time / 10000 * SOUND_VELOCITY / 2;

    // Reset the start time, this means that the signal was received
    // and a new wave can be triggered again
    ping_start_time = 0;
  }

  return;
}

void pico_init(void)
{
  gpio_init(TRIGGER_PIN);
  gpio_set_dir(TRIGGER_PIN, GPIO_OUT);

  gpio_init(ECHO_PIN);
  gpio_set_dir(ECHO_PIN, GPIO_IN);

  stdio_init_all(); // enables printf over USB

  gpio_set_irq_enabled_with_callback(ECHO_PIN,
                                     GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE,
                                     true, echo_callback);
  return;
}

void send_wave(void)
{
  // Trigger the wave
  gpio_put(TRIGGER_PIN, true);
  sleep_ms(10);
  gpio_put(TRIGGER_PIN, false);
}

int main(void)
{
  pico_init();

  while (true)
  {
    printf("Sending wave\n");
    send_wave();
    
    sleep_ms(100);
    
    printf("Last distance: %f\n", last_distance_cm);
  }
  return 0;
}
