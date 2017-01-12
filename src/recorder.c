/*************************************************************************
	> File Name: recorder.c
	> Author: weijie.yuan
	> Mail: yuanweijie1993@gmail.com
	> Created Time: Thu 12 Jan 2017 09:45:02 AM CST
 ************************************************************************/

#include <stdio.h>
#include <alsa/asoundlib.h>
#include "public.h"

static unsigned int channels = 1;
static unsigned int sample_rate = 16000;
static unsigned int frames = 32;
static char *device = "default";

int main(int argc,const char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr,"Usage:./aplayer xxx.pcm\n");
		return -1;
	}

	int rc;
    int dir;
	snd_pcm_t *handle;
	snd_pcm_hw_params_t *params;

	/* open devices */
	rc = snd_pcm_open(&handle,device,SND_PCM_STREAM_CAPTURE, 0); // open pcm device for capture
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
    snd_pcm_hw_params_set_rate(handle, params, sample_rate, 0);
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

    unsigned int val;
    long loops;

    snd_pcm_hw_params_get_period_time(params,&val,&dir);
    loops = 5000000 / val; // how many periods in 5s

    FILE *fp = fopen(argv[1],"wb");
    if (fp == NULL)
    {
    	fprintf(stderr,"open file: %s failed",argv[1]);
    	return -1;
    }

    while (loops)
    {
        loops--;
        rc = snd_pcm_readi(handle, buffer, frames);
    	if (rc == -EPIPE) // EPIPE means underrun
    	{
    		fprintf(stderr, "underrun occurred\n");
    		snd_pcm_prepare(handle);
    	}
    	else if (rc < 0)
    	{
    		fprintf(stderr,"error from readi: %s\n",snd_strerror(rc));
    		break;
    	}
    	else if (rc != (int)frames)
        	fprintf(stderr,"short write, write %d frames\n", rc);
    	
        /* read data */
    	rc = fwrite(buffer, 1, size, fp);
    	if (rc != size)
    	{
    	    fprintf(stderr,"write file: %s error\n", argv[1]);
    		break;
    	}
    }

    fclose(fp);
    snd_pcm_drain(handle); // send audio over completely
    snd_pcm_close(handle);
    afree(buffer);

    return 0;
}

