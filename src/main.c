#include <Windows.h>
#include <stdio.h>
#define _USE_MATH_DEFINES 
#include <math.h>
#include "wav.h"

#define SAMPLES_CNT 20000
#define BITS_PER_SAMPLE 16
#define CHANNELS_CNT 1
#define SAMPLE_RATE 2000

void print_wav_header(const wav_header *hdr);
void alloc_waveform(wav *wave, const wav_header *header, const void *data);
int main(int argc, char const *argv[]);

void print_wav_header(const wav_header *hdr){
    printf("riff: \t%.4s\n", hdr->riff);
    printf("chunk_size: \t%i\n", (int)hdr->chunk_size);
    printf("wave: \t%.4s\n", hdr->wave);
    printf("fmt: \t%s\n", hdr->fmt);
    printf("fmt_size: \t%i\n", (int)hdr->fmt_size);
    printf("fmt_type: \t%i\n", (int)hdr->fmt_type);
    printf("channels_cnt: \t%i\n", (int)hdr->channels_cnt);
    printf("sample_rate: \t%i\n", (int)hdr->sample_rate);
    printf("byte_rate: \t%i\n", (int)hdr->byte_rate);
    printf("block_align: \t%i\n", (int)hdr->block_align);
    printf("bits_per_sample: \t%i\n", (int)hdr->bits_per_sample);
    printf("data: \t%.4s\n", hdr->data);
    printf("data_size: \t%i\n", (int)hdr->data_size);
}

void alloc_waveform(wav *wave, const wav_header *header, const void *data){
    wave->header = *header;
    wave->data = malloc(header->data_size);
    memcpy(wave->data, data, wave->header.data_size);
}

int main(int argc, char const *argv[]){
    wav waveform;

    wav_read_file(&waveform, L"./test.wav", alloc_waveform);

    print_wav_header(&waveform.header);

    sndPlaySoundW((LPCWSTR)&waveform, SND_MEMORY | SND_SYNC );
    wav_save_file(L"./test.wav", &waveform);

    return 0;
}
