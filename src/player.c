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
	snd_pcm_hw_params_t *hw_params;

	/* open devices */
	rc = snd_pcm_open(&handle,device,SND_PCM_STREAM_PLAYBACK, 0); // open pcm device for playback
	if (rc < 0)
	{
		fprintf(stderr,"unable to open pcm device (%s)\n",snd_strerror(rc));
        return -1;
	}

	/* hardware params */
    rc = snd_pcm_hw_params_malloc(&hw_params); // malloc for params
    if (rc < 0)
    {
    	fprintf(stderr,"unable allocate hardware parameter structure (%s)\n",snd_strerror(rc));
        return -1;
    }

    rc = snd_pcm_hw_params_any(handle, hw_params); // initialize params
    if (rc < 0)
    {
    	fprintf(stderr,"cannot initialize hardware parameter structure (%s)\n",snd_strerror(rc));
        return -1;
    }

    rc = snd_pcm_hw_params_set_access(handle, hw_params, audio->access); // set access type
    if (rc < 0)
    {
    	fprintf(stderr,"cannot set access type (%s)\n",snd_strerror(rc));
        return -1;
    }
    rc = snd_pcm_hw_params_set_format(handle, hw_params, audio->format); //set format
    if (rc < 0)
    {
    	fprintf(stderr,"cannot set sample format (%s)\n",snd_strerror(rc));
        return -1;
    }
    rc = snd_pcm_hw_params_set_channels(handle, hw_params, audio->channels); //set channel(s)
    if (rc < 0)
    {
    	fprintf(stderr,"cannot set channel count (%s)\n",snd_strerror(rc));
        return -1;
    }
    rc = snd_pcm_hw_params_set_rate_near(handle, hw_params, &(audio->sample_rate), &dir);// set sample rate
    if (rc < 0)
    {
    	fprintf(stderr,"cannot set sample rate (%s)\n",snd_strerror(rc));
        return -1;
    }
    /*dir = audio->frames * 2;
    rc = snd_pcm_hw_params_set_buffer_size_near(handle, hw_params, &dir);
    if (rc < 0)
    {
    	fprintf(stderr,"cannot set sample rate (%s)\n",snd_strerror(rc));
        return -1;
    }*/

    audio->frames = 32;
    rc = snd_pcm_hw_params_set_period_size_near(handle, hw_params, (snd_pcm_uframes_t *)&(audio->frames), &dir);
    if (rc < 0)
    {
    	fprintf(stderr,"cannot set sample rate (%s)\n",snd_strerror(rc));
        return -1;
    }
    printf("hello\n");
    rc = snd_pcm_hw_params(handle, hw_params);
    if (rc < 0)
    {
        fprintf(stderr, "unable to set hw parameters: %s\n",snd_strerror(rc));
        return -1;
    }

    printf("Check if hardware supports pause: %d\n", snd_pcm_hw_params_can_pause(hw_params));

    snd_pcm_hw_params_get_period_size(hw_params, (snd_pcm_uframes_t *)&(audio->frames), &dir);
    snd_pcm_hw_params_free (hw_params);



    /* software params */
    snd_pcm_sw_params_t *sw_params;

    rc = snd_pcm_sw_params_malloc(&sw_params); // malloc for params
    if (rc < 0)
    {
    	fprintf(stderr,"unable allocate software parameter structure (%s)\n",snd_strerror(rc));
        return -1;
    }
    rc = snd_pcm_sw_params_current(handle, sw_params); // initialize params
    if (rc < 0)
    {
    	fprintf(stderr,"cannot initialize software parameters structure (%s)\n",snd_strerror(rc));
        return -1;
    }
    rc = snd_pcm_sw_params_set_start_threshold(handle, sw_params, audio->frames); // start playing when one period has been written
    if (rc < 0)
    {
    	fprintf(stderr,"cannot set start mode (%s)\n",snd_strerror(rc));
        return -1;
    }

    snd_pcm_uframes_t boundary;
    rc = snd_pcm_sw_params_get_boundary(sw_params,&boundary); // get boundary for ring pointers from a software configuration container.
    if (rc < 0)
    {
    	fprintf(stderr,"cannot get boundary (%s)\n",snd_strerror(rc));
        return -1;
    }

    rc = snd_pcm_sw_params_set_stop_threshold(handle, sw_params, boundary); // disable underrun reporting
    if (rc < 0)
	{
		fprintf(stderr,"cannot set stop threshold inside a software configuration container (%s)\n",snd_strerror(rc));
		return -1;
	}

    rc = snd_pcm_sw_params_set_silence_size(handle, sw_params, boundary ); // play silence when there is an underrun
    if (rc < 0)
	{
		fprintf(stderr,"cannot set silence size inside a software configuration container (%s)\n",snd_strerror(rc));
		return -1;
	}

    rc = snd_pcm_sw_params(handle, sw_params);
    if (rc < 0)
    {
        fprintf(stderr, "unable to set sw parameters: %s\n",snd_strerror(rc));
        return -1;
    }

    snd_pcm_sw_params_free(sw_params);



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
