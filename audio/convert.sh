#!/bin/sh

INPUT=test_audio.wav
OUTPUT=audio_data.raw

# Convert to Mono (-ac 1) and 22050Hz (-ar 22050)
ffmpeg -i $INPUT -ac 1 -ar 22050 -f s16le -acodec pcm_s16le $OUTPUT
