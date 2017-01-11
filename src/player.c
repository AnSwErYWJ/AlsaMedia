/*************************************************************************
	> File Name: player.c
	> Author: weijie.yuan
	> Mail: yuanweijie1993@gmail.com
	> Created Time: Wed 11 Jan 2017 02:50:13 PM CST
 ************************************************************************/

#include <stdio.h>
#include <alsa/asoundlib.h>
#include "public.h"

static unsigned int channels = 1;
static unsigned int sample_rate = 16000;
static unsigned int frames = 32;
static char *device = "default";
static char *audio = "../AudioResamplerate/audio/S16bit-LE-16kHz-Mono.pcm";

int main(void)
{
	int rc;
    int dir;
	snd_pcm_t *handle;
	snd_pcm_hw_params_t *params;

	/* open devices */
	rc = snd_pcm_open(&handle,device,SND_PCM_STREAM_PLAYBACK, 0); // open pcm device for playback
	if (rc < 0)
	{
		fprintf(stderr,"unable to open pcm device: %s\n",snd_strerror(rc));
        return -1;
	}

	/* init params */
    snd_pcm_hw_params_alloca(&params); // alloca params
    snd_pcm_hw_params_any(handle, params); // set default params

    /* set params */
    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED); // interleaved mode
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE); //format
    snd_pcm_hw_params_set_channels(handle, params, channels);
    snd_pcm_hw_params_set_rate_near(handle, params, &sample_rate, &dir);
    snd_pcm_hw_params_set_period_size_near(handle, params, (snd_pcm_uframes_t *)&frames, &dir);

    /* write the params to the driver */
    rc = snd_pcm_hw_params(handle, params);
    if (rc < 0)
    {
        fprintf(stderr, "unable to set hw parameters: %s\n",snd_strerror(rc));
        return -1;
    }

    /* calloc buffer for on period */
    snd_pcm_hw_params_get_period_size(params, (snd_pcm_uframes_t *)&frames, &dir);
    size_t size = frames * channels * (16/8); //sample*channels*bytes/(one channel)
    char *buffer = (char *) acalloc(1,size);

    FILE *fp = fopen("../AudioResamplerate/audio/S16bit-LE-16kHz-Mono.pcm","rb");
    if (fp == NULL)
    {
    	fprintf(stderr,"open file: %s failed",audio);
    	return -1;
    }

    while (1)
    {
    	/* read data */
    	rc = fread(buffer, 1, size, fp);
    	if (rc == 0) // not data
    	{
    		fprintf(stderr, "end of file: %s\n",audio);
    		break;
    	}
    	else if (rc != size)
    	{
    		if (feof(fp))
    			fprintf(stderr, "end of file: %s\n",audio);
    		else
    			fprintf(stderr,"read file: %s error\n", audio);

    		break;
    	}

    	/* write to driver for playback */
    	rc = snd_pcm_writei(handle, buffer, frames);
    	if (rc == -EPIPE) // EPIPE means underrun
    	{
    		fprintf(stderr, "underrun occurred\n");
    		snd_pcm_prepare(handle);
    	}
    	else if (rc < 0)
    	{
    		fprintf(stderr,"error from writei: %s\n",snd_strerror(rc));
    		return -1;
    	}
    	else if (rc != (int)frames)
        	fprintf(stderr,"short write, write %d frames\n", rc);
    }

    fclose(fp);
    snd_pcm_drain(handle); // send audio over completely
    snd_pcm_close(handle);
    afree(buffer);

    return 0;
}
