/*************************************************************************
	> File Name: player.c
	> Author: weijie.yuan
	> Mail: yuanweijie1993@gmail.com
	> Created Time: Wed 11 Jan 2017 02:50:13 PM CST
 ************************************************************************/

#include <stdio.h>
#include "public.h"

static char *device = "default";

int main(int argc,const char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr,"Usage:./aplayer audiofile\n");
		return -1;
	}

	/* config */
	audio_t *audio = (audio_t *)acalloc(1,sizeof(audio));
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
	rc = snd_pcm_open(&handle,device,SND_PCM_STREAM_PLAYBACK, 0); // open pcm device for playback
	if (rc < 0)
	{
		fprintf(stderr,"unable to open pcm device (%s)\n",snd_strerror(rc));
        return -1;
	}

	/* initialize params */
    rc = snd_pcm_hw_params_malloc(&params); // malloc for params
    if (rc < 0)
    {
    	fprintf(stderr,"unable allocate hardware parameter structure (%s)\n",snd_strerror(rc));
        return -1;
    }

    rc = snd_pcm_hw_params_any(handle, params); // initialize params
    if (rc < 0)
    {
    	fprintf(stderr,"cannot initialize hardware parameter structure (%s)\n",snd_strerror(rc));
        return -1;
    }

    /* set params */
    rc = snd_pcm_hw_params_set_access(handle, params, audio->access); // set access type
    if (rc < 0)
    {
    	fprintf(stderr,"cannot set access type (%s)\n",snd_strerror(rc));
        return -1;
    }
    rc = snd_pcm_hw_params_set_format(handle, params, audio->format); //set format
    if (rc < 0)
    {
    	fprintf(stderr,"cannot set sample format (%s)\n",snd_strerror(rc));
        return -1;
    }
    rc = snd_pcm_hw_params_set_channels(handle, params, audio->channels); //set channel(s)
    if (rc < 0)
    {
    	fprintf(stderr,"cannot set channel count (%s)\n",snd_strerror(rc));
        return -1;
    }
    rc = snd_pcm_hw_params_set_rate_near(handle, params, &(audio->sample_rate), &dir);// set sample rate
    if (rc < 0)
    {
    	fprintf(stderr,"cannot set sample rate (%s)\n",snd_strerror(rc));
        return -1;
    }
    dir = audio->frames * 2;
    rc = snd_pcm_hw_params_set_buffer_size_near(handle, params, &dir);
    if (rc < 0)
    {
    	fprintf(stderr,"cannot set sample rate (%s)\n",snd_strerror(rc));
        return -1;
    }
    rc = snd_pcm_hw_params_set_period_size_near(handle, params, (snd_pcm_uframes_t *)&(audio->frames), &dir);
    if (rc < 0)
    {
    	fprintf(stderr,"cannot set sample rate (%s)\n",snd_strerror(rc));
        return -1;
    }

    /* write the params to the driver */
    rc = snd_pcm_hw_params(handle, params);
    if (rc < 0)
    {
        fprintf(stderr, "unable to set hw parameters: %s\n",snd_strerror(rc));
        return -1;
    }

    printf("Check if hardware supports pause: %d\n", snd_pcm_hw_params_can_pause(params));

    /* calloc buffer for on period */
    snd_pcm_hw_params_get_period_size(params, (snd_pcm_uframes_t *)&(audio->frames), &dir);
    size_t size = audio->frames * audio->channels * (audio->sample/8); //sample*channels*bytes/(one channel)
    char *buffer = (char *) acalloc(1,size);

    FILE *fp = fopen(argv[1],"rb");
    if (fp == NULL)
    {
    	fprintf(stderr,"open file: %s failed",argv[1]);
    	return -1;
    }

    while (1)
    {
    	/* read data */
    	rc = fread(buffer, 1, size, fp);
    	if (rc == 0) // not data
    	{
    		//fprintf(stderr, "end of file: %s\n",argv[1]);
    		break;
    	}
    	else if (rc != size)
    	{
    		if (feof(fp))
    		{}	//fprintf(stderr, "end of file: %s\n",argv[1]);
    		else
    			fprintf(stderr,"read file: %s error\n", argv[1]);

    		break;
    	}

    	/* write to driver for playback */
    	rc = snd_pcm_writei(handle, buffer, audio->frames);
    	if (rc == -EPIPE) // EPIPE means underrun
    	{
    		fprintf(stderr, "underrun occurred\n");
    		snd_pcm_prepare(handle);
    	}
    	else if (rc < 0)
    	{
    		fprintf(stderr,"error from writei: %s\n",snd_strerror(rc));
    		break;
    	}
    	else if (rc != (int)audio->frames)
        	fprintf(stderr,"short write, write %d frames\n", rc);
    }

    fclose(fp);
    snd_pcm_drain(handle); // send audio over completely
    snd_pcm_close(handle);
    afree(buffer);
    afree(audio);

    return 0;
}
