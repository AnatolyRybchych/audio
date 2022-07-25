#ifndef __WAWE_H_
#define __WAWE_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct wav_header{
    uint8_t riff[4]; 
    uint32_t chunk_size;
    uint8_t wave[4];
    uint8_t fmt[4];
    uint32_t fmt_size;
    uint16_t fmt_type;
    uint16_t channels_cnt;
    uint32_t sample_rate;//hertz
    uint32_t byte_rate;//
    uint16_t block_align;//bytes_per_sample * channels_cnt
    uint16_t bits_per_sample;
    uint8_t data[4];
    uint32_t data_size;
} wav_header;

typedef struct wav{
    wav_header header;
    void *data;
} wav;

void wav_write_header(wav_header *wav, uint16_t channels_cnt, uint32_t sample_rate, uint16_t bits_per_sample, uint32_t data_size);
bool wav_save_file(const wchar_t *filename, const wav *wave);

#endif //__WAWE_H_
