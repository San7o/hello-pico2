  .section .rodata
  .global audio_data
  .global audio_data_end

audio_data:
  .incbin "audio/audio_data.raw"
audio_data_end: 
