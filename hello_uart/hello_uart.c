// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <pico/stdlib.h>

//
// hello_uart.c
// ============
//
// Send a string via uart.
//
// Setup
// -----
//
// UART is a serial communication protocol which uses two lines, TX
// and RX.
// Both the writer and the receiver have to use the same baud
// rate which is the speed of the communication.
#define BAUD_RATE 115200

// We need a UART to USB converter for this project. I am using the
// FT232RL module, a description can be found in the datasheets/
// directory.
//
// As for the connections, we need to connect the module's RX port to
// the Pi's GP0, and TX to GP1. Then the code is straight forward.

int main(void)
{
  stdio_init_all();
  
  // Set the GPIO pin mux to the UART
  //   pin 0 is TC, 1 is RX
  // Note use of UART_FUNCSEL_NUM for the general case where the func sel
  // used for UART depends on the pin number.
  // Do this before calling uart_init to avoid losing data
  gpio_set_function(0, UART_FUNCSEL_NUM(uart0, 0));
  gpio_set_function(1, UART_FUNCSEL_NUM(uart0, 1));

  // Initialize UART 0
  uart_init(uart0, BAUD_RATE);
  while (true)
  {
    // Here we use both \r and \n to go at the start of the new line
    uart_puts(uart0, "Hello, world!\r\n");
    sleep_ms(1000);
  }
  return 0;
}

//
// Once we plug the USB from the module, we should see a new tty in
// /dev/ttyUSB*. We may read the output with 'cat', or use 'minicom'
// for more control:
//
//    sudo minicom -b 115200 -D /dev/ttyUSB0
//
