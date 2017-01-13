/*************************************************************************
	> File Name: public.h
	> Author: weijie.yuan
	> Mail: yuanweijie1993@gmail.com
	> Created Time: Wed 11 Jan 2017 02:48:27 PM CST
 ************************************************************************/

#ifndef _PUBLIC_H
#define _PUBLIC_H

#include <alsa/asoundlib.h>

typedef struct _audio_config {
    unsigned int channels;
    unsigned int sample_rate;
    unsigned int frames; //frames per period
    snd_pcm_format_t format;
    snd_pcm_access_t access;
    unsigned int sample;
}audio_t;

void display_alsalib();
void display_info(snd_pcm_t *handle,snd_pcm_hw_params_t *params);
void *acalloc(size_t n,size_t size);
void afree(void *mem);

#endif
