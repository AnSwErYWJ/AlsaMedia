#include "import.h"

/*
 *  dump hardware parameters
 */
void dump_hw_params(snd_pcm_t *handle, snd_pcm_hw_params_t *params)
{
    unsigned int i = 0, j = 0;
    int dir = 0;
    snd_pcm_access_t access = -1;
    snd_pcm_format_t format = -1;
    snd_pcm_subformat_t subformat = -1;
    snd_pcm_uframes_t frames = -1;

    printf("\nDump hardware parameters of PCM handle '%s'\n", snd_pcm_name(handle));

    printf("  PCM state = %s\n", snd_pcm_state_name(snd_pcm_state(handle)));

    snd_pcm_hw_params_get_access(params, &access);
    printf("  access type = %s\n", snd_pcm_access_name(access));

    snd_pcm_hw_params_get_format(params, &format);
    printf("  format = '%s' (%s)\n", snd_pcm_format_name(format), snd_pcm_format_description(format));

    snd_pcm_hw_params_get_subformat(params, &subformat);
    printf("  subformat = '%s' (%s)\n", snd_pcm_subformat_name(subformat), snd_pcm_subformat_description(subformat));

    snd_pcm_hw_params_get_channels(params, &i);
    printf("  channels = %d\n", i);

    snd_pcm_hw_params_get_rate(params, &i, &dir);
    printf("  rate = %d bps\n", i);

    snd_pcm_hw_params_get_period_time(params, &i, &dir);
    printf("  period time = %d us\n", i);

    snd_pcm_hw_params_get_period_size(params, &frames, &dir);
    printf("  period size = %d frames\n", (int)frames);

    snd_pcm_hw_params_get_buffer_time(params, &i, &dir);
    printf("  buffer time = %d us\n", i);

    snd_pcm_hw_params_get_buffer_size(params, &frames);
    printf("  buffer size = %d frames\n", (int)frames);

    snd_pcm_hw_params_get_periods(params, &i, &dir);
    printf("  periods per buffer = %d frames\n", i);

    snd_pcm_hw_params_get_rate_numden(params, &i, &j);
    printf("  exact rate = %d/%d bps\n", i, j);

    i = snd_pcm_hw_params_get_sbits(params);
    printf("  significant bits = %d\n", i);

    i = snd_pcm_hw_params_is_batch(params);
    printf("  is batch = %d\n", i);

    i = snd_pcm_hw_params_is_block_transfer(params);
    printf("  is block transfer = %d\n", i);

    i = snd_pcm_hw_params_is_double(params);
    printf("  is double = %d\n", i);

    i = snd_pcm_hw_params_is_half_duplex(params);
    printf("  is half duplex = %d\n", i);

    i = snd_pcm_hw_params_is_joint_duplex(params);
    printf("  is joint duplex = %d\n", i);

    i = snd_pcm_hw_params_can_overrange(params);
    printf("  can overrange = %d\n", i);

    i = snd_pcm_hw_params_can_mmap_sample_resolution(params);
    printf("  can mmap = %d\n", i);

    i = snd_pcm_hw_params_can_pause(params);
    printf("  can pause = %d\n", i);

    i = snd_pcm_hw_params_can_resume(params);
    printf("  can resume = %d\n", i);

    i = snd_pcm_hw_params_can_sync_start(params);
    printf("  can sync start = %d\n", i);

    printf("\n");
}

int main(int argc, const char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: ./aplayer xxx.pcm\n");
		return -1;
	}

	int rc = 0;
    int dir = 0;
	snd_pcm_t *handle = NULL;
	snd_pcm_hw_params_t *hw_params = NULL;
    snd_pcm_sw_params_t *sw_params = NULL;
    snd_pcm_uframes_t boundary;
    size_t size = 0;
    char *buffer = NULL;
    FILE *fp = NULL;
	
    /* init config */
    char *device = "default";
    unsigned int channels = 1;
    unsigned int sample_rate = 16000;
    snd_pcm_uframes_t frames = 32; //frames per period
    snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;
    snd_pcm_access_t access = SND_PCM_ACCESS_RW_INTERLEAVED;
    unsigned int sample = 16;

    /* open pcm device for playback */ 
	rc = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0); 
	if (rc < 0)
	{
		fprintf(stderr, "unable to open pcm device (%s)\n", snd_strerror(rc));
        rc = -1;
        goto END;
	}

    printf("open pcm device successfully\n");

	/* malloc for hardware params */
    rc = snd_pcm_hw_params_malloc(&hw_params);
    if (rc < 0)
    {
    	fprintf(stderr, "unable malloc hardware parameter structure (%s)\n", snd_strerror(rc));
        rc = -1;
        goto END;
    }

	/* initialize hardware params */
    rc = snd_pcm_hw_params_any(handle, hw_params);
    if (rc < 0)
    {
    	fprintf(stderr, "unable initialize hardware parameter structure (%s)\n", snd_strerror(rc));
        rc = -1;
        goto END;
    }

    /* set the desired hardware parameters */
    rc = snd_pcm_hw_params_set_access(handle, hw_params, access);
    if (rc < 0)
    {
    	fprintf(stderr, "unable set access type (%s)\n",snd_strerror(rc));
        rc = -1;
        goto END;
    }

    rc = snd_pcm_hw_params_set_format(handle, hw_params, format);
    if (rc < 0)
    {
    	fprintf(stderr, "unable set sample format (%s)\n", snd_strerror(rc));
        rc = -1;
        goto END;
    }

    rc = snd_pcm_hw_params_set_channels(handle, hw_params, channels);
    if (rc < 0)
    {
    	fprintf(stderr, "unable set channel count (%s)\n", snd_strerror(rc));
        rc = -1;
        goto END;
    }

    rc = snd_pcm_hw_params_set_rate_near(handle, hw_params, &sample_rate, &dir);
    if (rc < 0)
    {
    	fprintf(stderr, "unable set sample rate (%s)\n", snd_strerror(rc));
        rc = -1;
        goto END;
    }

    rc = snd_pcm_hw_params_set_period_size_near(handle, hw_params, &frames, &dir);
    if (rc < 0)
    {
    	fprintf(stderr, "unable set period size (%s)\n", snd_strerror(rc));
        rc = -1;
        goto END;
    }

    rc = snd_pcm_hw_params(handle, hw_params);
    if (rc < 0)
    {
        fprintf(stderr, "unable to set hw parameters: %s\n", snd_strerror(rc));
        rc = -1;
        goto END;
    }

    printf("set pcm hardware parameters successfully, if hardware supports pause: %d\n", snd_pcm_hw_params_can_pause(hw_params));

    /* dump hardware parameters */
    dump_hw_params(handle, hw_params);

    /* use a buffer large enough to hold one period */
    snd_pcm_hw_params_get_period_size(hw_params, &frames, &dir); 

	/* malloc for software params */
    rc = snd_pcm_sw_params_malloc(&sw_params);
    if (rc < 0)
    {
    	fprintf(stderr, "unable allocate software parameter structure (%s)\n", snd_strerror(rc));
        rc = -1;
        goto END;
    }

	/* initialize software params */
    rc = snd_pcm_sw_params_current(handle, sw_params);
    if (rc < 0)
    {
    	fprintf(stderr, "unable initialize software parameters structure (%s)\n", snd_strerror(rc));
        rc = -1;
        goto END;
    }

    /* set the desired software parameters */
    rc = snd_pcm_sw_params_set_start_threshold(handle, sw_params, frames);
    if (rc < 0)
    {
    	fprintf(stderr, "unable set start mode (%s)\n", snd_strerror(rc));
        rc = -1;
        goto END;
    }

    rc = snd_pcm_sw_params_get_boundary(sw_params, &boundary); 
    if (rc < 0)
    {
    	fprintf(stderr, "unable get boundary (%s)\n", snd_strerror(rc));
        rc = -1;
        goto END;
    }

    rc = snd_pcm_sw_params_set_stop_threshold(handle, sw_params, boundary);
    if (rc < 0)
	{
		fprintf(stderr, "unable set stop threshold inside a software configuration container (%s)\n", snd_strerror(rc));
        rc = -1;
        goto END;
	}

    rc = snd_pcm_sw_params_set_silence_size(handle, sw_params, boundary);
    if (rc < 0)
	{
		fprintf(stderr, "unable set silence size inside a software configuration container (%s)\n", snd_strerror(rc));
        rc = -1;
        goto END;
	}

    rc = snd_pcm_sw_params(handle, sw_params); 
    if (rc < 0)
    {
        fprintf(stderr, "unable to set sw parameters: %s\n", snd_strerror(rc));
        rc = -1;
        goto END;
    }

    printf("set pcm software parameters successfully\n");

    size = frames * channels * (sample / 8);
    buffer = (char *)calloc(1, size);
    if (NULL == buffer)
    {
        fprintf(stderr, "unable malloc buffer\n");
        rc = -1;
        goto END;
    }

    fp = fopen(argv[1], "rb");
    if (NULL == fp)
    {
    	fprintf(stderr, "unable open file: %s\n", argv[1]);
        rc = -1;
        goto END;
    }

    printf("play %s start\n", argv[1]);

    while (1)
    {
    	/* read data frome file or input */
    	rc = fread(buffer, 1, size, fp);
    	if (0 == rc)
    	{
    		//printf("end of file\n");
    		break;
    	}
    	else if (rc != size)
    	{
    		if (feof(fp))
    		{
    		    //printf("end of file\n");
                break;
            }
    		else
            {
    			fprintf(stderr,"read file: %s error\n", argv[1]);
            }
    	}

    	/* write to driver for playback */
    	rc = snd_pcm_writei(handle, buffer, frames);
    	if (rc == -EPIPE)
    	{
    		fprintf(stderr, "underrun occurred\n");
    		snd_pcm_prepare(handle);
    	}
    	else if (rc < 0)
    	{
    		fprintf(stderr, "error from writei: %s\n", snd_strerror(rc));
    		break;
    	}
    	else if (rc != frames)
        {
        	fprintf(stderr, "short write, write %d frames\n", rc);
        }
    }

    /* send audio over completely */
    snd_pcm_drain(handle);

    printf("play end\n");

    rc = 0;
END:
    if (NULL != hw_params)
    {
        snd_pcm_hw_params_free(hw_params);
        hw_params = NULL;
    }

    if (NULL != sw_params)
    {
        snd_pcm_sw_params_free(sw_params);
        sw_params = NULL;
    }

    if (NULL != fp)
    {
        fclose(fp);
        fp = NULL;
    }

    if (NULL != handle)
    {
        snd_pcm_close(handle);
        handle = NULL;
    }

    if (NULL != buffer)
    {
        free(buffer);
        buffer = NULL;
    }

    /*  releases all resources of the global configuration tree */
    snd_config_update_free_global();
    
    return rc;
}
