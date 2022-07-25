#include "wav.h"
#include <stdio.h>

#define fwrite_arr(_struct, field, file) fwrite((_struct).field, sizeof((_struct).field[0]), sizeof((_struct).field) / sizeof((_struct).field[0]), file)
#define fwrite_element(_struct, element, file) fwrite(&(_struct).element, sizeof((_struct).element), 1, file)

void wav_write_header(wav_header *wav, uint16_t channels_cnt, uint32_t sample_rate, uint16_t bits_per_sample, uint32_t data_size){
    wav->riff[0] = 'R';
    wav->riff[1] = 'I';
    wav->riff[2] = 'F';
    wav->riff[3] = 'F';

    wav->chunk_size = data_size + sizeof(wav_header) - 8;

    wav->wave[0] = 'W';
    wav->wave[1] = 'A';
    wav->wave[2] = 'V';
    wav->wave[3] = 'E';

    wav->fmt[0] = 'f';
    wav->fmt[1] = 'm';
    wav->fmt[2] = 't';
    wav->fmt[3] = 0x20;
    
    wav->fmt_size = 16;

    wav->fmt_type = 1;

    wav->channels_cnt = channels_cnt;

    wav->sample_rate = sample_rate;

    wav->byte_rate = (sample_rate * bits_per_sample * channels_cnt) / 8;

    wav->block_align = (bits_per_sample * channels_cnt) / 8;

    wav->bits_per_sample = bits_per_sample;

    wav->data[0] = 'd';
    wav->data[1] = 'a';
    wav->data[2] = 't';
    wav->data[3] = 'a';

    wav->data_size = data_size;
}

bool wav_save_file(const wchar_t *filename, const wav *wave){
    FILE *file = _wfopen(filename, L"w");
    if(file == NULL) return false;

    fwrite_arr(wave->header, riff, file);
    fwrite_element(wave->header, chunk_size, file);
    fwrite_arr(wave->header, wave, file);
    fwrite_arr(wave->header, fmt, file);
    fwrite_element(wave->header, fmt_size, file);
    fwrite_element(wave->header, fmt_type, file);
    fwrite_element(wave->header, channels_cnt, file);
    fwrite_element(wave->header, sample_rate, file);
    fwrite_element(wave->header, byte_rate, file);
    fwrite_element(wave->header, block_align, file);
    fwrite_element(wave->header, bits_per_sample, file);
    fwrite_arr(wave->header, data, file);
    fwrite_element(wave->header, data_size, file);
    fwrite(wave->data, wave->header.block_align, wave->header.data_size / wave->header.block_align, file);
    return true;
}

