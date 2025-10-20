#include <stdio.h>
#include "pico/stdlib.h"

#ifndef LED_DELAY_MS
#define LED_DELAY_MS (1000 * 60 * 60)
#endif

#ifndef PICO_DEFAULT_LED_PIN
#warning hello_led example requires a board with a regular LED
#endif

void pico_led_init(void)
{
#ifdef PICO_DEFAULT_LED_PIN
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
#endif
    return;
}

void pico_set_led(bool led_on)
{
#ifdef PICO_DEFAULT_LED_PIN
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#endif
    return;
}

int main(void)
{
    pico_led_init();
    pico_set_led(false);
    sleep_ms(LED_DELAY_MS);
    pico_set_led(true);
    while (true)
    {
        sleep_ms(1000);
    }
    return 0;
}
