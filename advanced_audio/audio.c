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

// TODO

#include <stdio.h>
#include <math.h>
#include <pico/stdlib.h>
#include <pico/audio_i2s.h>

// LCK must be BCK + 1
#define BCK_GPIO_PIN   6   // Audio data base block pin
#define LCK_GPIO_PIN   7   // Audio data clock input (base + 1)
#define DIN_GPIO_PIN   8   // Audio data pin

#define AUDIO_FREQUENCY      22050 // Hz
#define SAMPLES_PER_BUFFER   256

extern const int16_t audio_data[];
extern const int16_t audio_data_end[];

static audio_buffer_pool_t* pico_init(void)
{
  // Init internal LED
  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
  
  audio_format_t intended_format = {
    .sample_freq = AUDIO_FREQUENCY,
    .format = AUDIO_BUFFER_FORMAT_PCM_S16,
    .channel_count = 2, // 2 bytes per sample * 2 channels = 4
  };

  audio_buffer_format_t producer_format = {
    .format        = &intended_format,
    .sample_stride = 4,
  };
  
  audio_buffer_pool_t *producer_pool =
    audio_new_producer_pool(&producer_format,
                            3, // buffer count
                            SAMPLES_PER_BUFFER // buffer sample count
                            );

  audio_i2s_config_t config = {
    .data_pin       = DIN_GPIO_PIN,
    .clock_pin_base = BCK_GPIO_PIN,
    .dma_channel    = 0,
    .pio_sm         = 0,
  };

  const audio_format_t *format = audio_i2s_setup(&intended_format,
                                                 &config);
  if (!format)
    return NULL;

  if (!audio_i2s_connect(producer_pool))
    return NULL;
  
  audio_i2s_set_enabled(true);

  return producer_pool;
}

int main(void)
{
  audio_buffer_pool_t *ap = pico_init();
  if (!ap)
  {
    // Blink the internal LED on error
    while(true)
    {
      gpio_put(PICO_DEFAULT_LED_PIN, true);
      sleep_ms(200);
      gpio_put(PICO_DEFAULT_LED_PIN, false);
      sleep_ms(200);
    }
    return 1;
  }

  float frequency = 440.0f;
  uint32_t total_samples = (uint32_t)(audio_data_end - audio_data);
  uint32_t step = (uint32_t)((frequency * total_samples / (float)AUDIO_FREQUENCY) * 65536.0f);
  uint32_t current_sample_index = 0;

  float    volume  = 0.05f;

  gpio_put(PICO_DEFAULT_LED_PIN, true);
  
  while (true)
  {
    struct audio_buffer *buffer = take_audio_buffer(ap, true);
    int16_t *samples = (int16_t*) buffer->buffer->bytes;
    for (uint i = 0; i < buffer->max_sample_count; ++i)
    {
      if (current_sample_index < total_samples)
      {
        int32_t val = (int16_t)(audio_data[current_sample_index++] * volume);

        samples[i * 2] = val;      // Left channel
        samples[i * 2 + 1] = val;  // Right channel
      }
      else
      {
        samples[i * 2] = 0;  // silence if file ends
        samples[i * 2 + 1] = 0;
        // current_sample_index = 0;
      }
    }
    buffer->sample_count = buffer->max_sample_count;
    give_audio_buffer(ap, buffer);
  }
  return 0;
}
