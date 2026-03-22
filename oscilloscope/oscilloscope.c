// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <stdio.h>
#include <unistd.h>
#include <pico/stdlib.h>
#include <hardware/adc.h>
#include <hardware/dma.h>
#include <hardware/clocks.h>
#include <hardware/pwm.h>

#define PIN_ANALOG_IN 26

#define SAMPLES_FREQ_HZ 44100
#define SAMPLES_BUFF_SIZE 1024
uint16_t samples_buffer_a[SAMPLES_BUFF_SIZE] = {0};
uint16_t samples_buffer_b[SAMPLES_BUFF_SIZE] = {0};
volatile bool buffer_a_ready = false;
volatile bool buffer_b_ready = false;

int chan_a, chan_b;

void dma_handler(void)
{
  if (dma_hw->intr & (1u << chan_a))
  {
    dma_hw->ints0 = (1u << chan_a); // Clear the interrupt
    buffer_a_ready = true;
    dma_channel_set_write_addr(chan_a, samples_buffer_a, false);
  }
  if (dma_hw->intr & (1u << chan_b))
  {
    dma_hw->ints0 = (1u << chan_b); // Clear the interrupt
    buffer_b_ready = true;
    dma_channel_set_write_addr(chan_b, samples_buffer_b, false);
  }
}

void pico_init(void)
{
  stdio_init_all();

  // PWM
  //
  // We simulate real analog signal in GPIO0. Connect it to
  // PIN_ANALOG_IN with a jumper to see the signal.
  
  gpio_set_function(0, GPIO_FUNC_PWM);
  uint slice_num = pwm_gpio_to_slice_num(0);
  pwm_set_wrap(slice_num, 12500);
  pwm_set_chan_level(slice_num, PWM_CHAN_A, 6250);
  pwm_set_enabled(slice_num, true);
  
  // ADC
  
  adc_init();
  adc_gpio_init(PIN_ANALOG_IN);
  adc_select_input(0);
  adc_fifo_setup(
                 true,   // Enable FIFO
                 true,   // Enable DMA request
                 1,      // DREQ treshold
                 false,  // No error bit
                 false  // keep 12 bits
                 );

  uint32_t clock_freq_hz = clock_get_hz(clk_sys);
  adc_set_clkdiv((float)clock_freq_hz / SAMPLES_FREQ_HZ - 1.0f);

  // DMA
  
  chan_a = dma_claim_unused_channel(true);
  chan_b = dma_claim_unused_channel(true);
  
  dma_channel_config cfg_a = dma_channel_get_default_config(chan_a);
  channel_config_set_transfer_data_size(&cfg_a, DMA_SIZE_16);
  channel_config_set_read_increment(&cfg_a, false); // Always read the the same index
  channel_config_set_write_increment(&cfg_a, true); // Write in positions one after the other
  channel_config_set_chain_to(&cfg_a, chan_b);
  channel_config_set_dreq(&cfg_a, DREQ_ADC); // Synchronized with ADC
  dma_channel_configure(
                        chan_a, &cfg_a,
                        samples_buffer_a,   // Destination
                        &adc_hw->fifo,      // Source (ADC)
                        SAMPLES_BUFF_SIZE,
                        false               // Do not start immediately
                        );


  dma_channel_config cfg_b = dma_channel_get_default_config(chan_b);
  channel_config_set_transfer_data_size(&cfg_b, DMA_SIZE_16);
  channel_config_set_read_increment(&cfg_b, false); // Always read the the same index
  channel_config_set_write_increment(&cfg_b, true); // Write in positions one after the other
  channel_config_set_chain_to(&cfg_b, chan_a);
  channel_config_set_dreq(&cfg_b, DREQ_ADC); // Synchronized with ADC
  dma_channel_configure(
                        chan_b, &cfg_b,
                        samples_buffer_b,   // Destination
                        &adc_hw->fifo,      // Source (ADC)
                        SAMPLES_BUFF_SIZE,
                        false               // Do not start immediately
                        );

  // IRQ

  dma_set_irq0_channel_mask_enabled((1u << chan_a) | (1u << chan_b), true);
  irq_set_exclusive_handler(DMA_IRQ_0, dma_handler);
  irq_set_enabled(DMA_IRQ_0, true);
  
  adc_run(true);
  dma_channel_start(chan_a);
  
  return;
}

int main(void)
{
  pico_init();
  
  while (true)
  {
    if (buffer_a_ready)
    {
      fwrite(samples_buffer_a, sizeof(uint16_t), SAMPLES_BUFF_SIZE, stdout);
      fflush(stdout);
      buffer_a_ready = false;
    }
    if (buffer_b_ready)
    {
      fwrite(samples_buffer_b, sizeof(uint16_t), SAMPLES_BUFF_SIZE, stdout);
      fflush(stdout);
      buffer_b_ready = false;
    }
  }
  
  return 0;
}
