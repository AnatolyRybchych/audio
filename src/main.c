#include <Windows.h>
#include <stdio.h>
#define _USE_MATH_DEFINES 
#include <math.h>
#include "wav.h"

#define SAMPLES_CNT 20000
#define BITS_PER_SAMPLE 16
#define CHANNELS_CNT 1
#define SAMPLE_RATE 2000

int main(int argc, char const *argv[]){

    int16_t samples[SAMPLES_CNT * CHANNELS_CNT];

    for(int s = 0; s < SAMPLES_CNT; s++){
        double val = sin((double)s * M_PI * 200 / SAMPLE_RATE);

        for(int c = 0 ; c < CHANNELS_CNT; c++){
            samples[s * CHANNELS_CNT + c] = INT16_MAX * 0.9 * val; 

            long int sum = 0;
            for(int i = 0; i < s; i++)
                sum += samples[s * CHANNELS_CNT + c];
            
            samples[s * CHANNELS_CNT + c] += sin((double)sum / (double)SAMPLE_RATE * M_PI) * INT16_MAX;
            samples[s * CHANNELS_CNT + c] = sin(samples[s * CHANNELS_CNT + c] * 0.0001) * INT16_MAX;
        }
    }

    wav waveform;

    waveform.data = &samples;
    wav_write_header(&waveform.header, CHANNELS_CNT, SAMPLE_RATE, BITS_PER_SAMPLE, CHANNELS_CNT * BITS_PER_SAMPLE * SAMPLES_CNT / 8);

    printf("riff: \t%.4s\n", waveform.header.riff);
    printf("chunk_size: \t%i\n", (int)waveform.header.chunk_size);
    printf("wave: \t%.4s\n", waveform.header.wave);
    printf("fmt: \t%s\n", waveform.header.fmt);
    printf("fmt_size: \t%i\n", (int)waveform.header.fmt_size);
    printf("fmt_type: \t%i\n", (int)waveform.header.fmt_type);
    printf("channels_cnt: \t%i\n", (int)waveform.header.channels_cnt);
    printf("sample_rate: \t%i\n", (int)waveform.header.sample_rate);
    printf("byte_rate: \t%i\n", (int)waveform.header.byte_rate);
    printf("block_align: \t%i\n", (int)waveform.header.block_align);
    printf("bits_per_sample: \t%i\n", (int)waveform.header.bits_per_sample);
    printf("data: \t%.4s\n", waveform.header.data);
    printf("data_size: \t%i\n", (int)waveform.header.data_size);

    sndPlaySound((LPCSTR)&waveform, SND_MEMORY | SND_SYNC );
    wav_save_file(L"./test.wav", &waveform);


    return 0;
}
