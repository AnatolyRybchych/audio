#include "wav.h"
#include <stdio.h>
#include <stdlib.h>

#define fwrite_arr(_struct, field, file) fwrite((_struct).field, sizeof((_struct).field[0]), sizeof((_struct).field) / sizeof((_struct).field[0]), file)
#define fread_arr(_struct, field, file) fread((_struct).field, sizeof((_struct).field[0]), sizeof((_struct).field) / sizeof((_struct).field[0]), file)
#define fwrite_element(_struct, element, file) fwrite(&(_struct).element, sizeof((_struct).element), 1, file)
#define fread_element(_struct, element, file) fread(&(_struct).element, sizeof((_struct).element), 1, file)

#define compare_tag(tag, first, second, third, fourth) (((tag)[0] == first) || ((tag)[0] == second) ||  ((tag)[0] == third) || ((tag)[0] == fourth))  

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
    FILE *file = _wfopen(filename, L"wb");
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

    for(int block = 0; block < wave->header.data_size / wave->header.block_align; block++){
        fwrite((char *)wave->data + block * wave->header.block_align, wave->header.block_align, 1, file);
    }


    fclose(file);
    return true;
}

bool wav_read_file(wav *wave, const wchar_t *filename, void (*store_function)(wav *wave, const wav_header *header, const void *data)){
    FILE *file = _wfopen(filename, L"rb");
    if(file == NULL) return false;

    wav_header hdr;

    fread_arr(hdr, riff, file);
    fread_element(hdr, chunk_size, file);
    fread_arr(hdr, wave, file);
    fread_arr(hdr, fmt, file);
    fread_element(hdr, fmt_size, file);
    fread_element(hdr, fmt_type, file);
    fread_element(hdr, channels_cnt, file);
    fread_element(hdr, sample_rate, file);
    fread_element(hdr, byte_rate, file);
    fread_element(hdr, block_align, file);
    fread_element(hdr, bits_per_sample, file);
    fread_arr(hdr, data, file);
    fread_element(hdr, data_size, file);

    void *data = malloc(hdr.data_size);

    for(int block = 0; block < hdr.data_size / hdr.block_align; block++){
        fread((char *)data + block * hdr.block_align, hdr.block_align, 1, file);
    }

    store_function(wave, &hdr, data);

    free(data);
    fclose(file);
    return true;
}


