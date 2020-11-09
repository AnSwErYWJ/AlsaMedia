/*************************************************************************
	> File Name: recorder.c
	> Author: weijie.yuan
	> Mail: yuanweijie1993@gmail.com
	> Created Time: Thu 12 Jan 2017 09:45:02 AM CST
 ************************************************************************/

#include <stdio.h>
#include "public.h"

static char *device = "default";

int main(int argc,const char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr,"Usage:./arecorder audiofile\n");
		return -1;
	}

	/* config */
	audio_t *audio = (audio_t *)acalloc(1,sizeof(audio_t));
	audio->channels = 1;
	audio->sample_rate = 16000;
	audio->frames = 32;
	audio->format = SND_PCM_FORMAT_S16_LE;
	audio->access = SND_PCM_ACCESS_RW_INTERLEAVED;
	audio->sample = 16;

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
    snd_pcm_hw_params_set_access(handle, params, audio->access); // interleaved mode
    snd_pcm_hw_params_set_format(handle, params, audio->format); //format
    snd_pcm_hw_params_set_channels(handle, params, audio->channels);
    snd_pcm_hw_params_set_rate(handle, params, audio->sample_rate, 0);
    snd_pcm_hw_params_set_period_size_near(handle, params, (snd_pcm_uframes_t *)&(audio->frames), &dir);

    /* write the params to the driver */
    rc = snd_pcm_hw_params(handle, params);
    if (rc < 0)
    {
        fprintf(stderr, "unable to set hw parameters: %s\n",snd_strerror(rc));
        return -1;
    }

    /* calloc buffer for on period */
    snd_pcm_hw_params_get_period_size(params, (snd_pcm_uframes_t *)&(audio->frames), &dir);
    size_t size = (audio->frames) * audio->channels * (audio->sample/8); //sample*channels*bytes/(one channel)
    char *buffer = (char *) acalloc(1,size);

    //unsigned int val;
    //long loops;

    //snd_pcm_hw_params_get_period_time(params,&val,&dir);
    //loops = 5000000 / val; // how many periods in 5s

    FILE *fp = fopen(argv[1],"wb");
    if (fp == NULL)
    {
    	fprintf(stderr,"open file: %s failed",argv[1]);
    	return -1;
    }

    //while (loops)
    while (1)
    {
        //loops--;
        rc = snd_pcm_readi(handle, buffer, audio->frames);
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
    	else if (rc != (int)audio->frames)
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
    afree(audio);

    return 0;
}

