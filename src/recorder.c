#include "import.h"

int main(int argc, const char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: ./recorder xxx.pcm\n");
		return -1;
	}

	int rc = 0;
    int dir = 0;
	snd_pcm_t *handle = NULL;
	snd_pcm_hw_params_t *hw_params = NULL;
	size_t size = 0;
    char *buffer = NULL;
	long loops = 0;
	FILE *fp = NULL;
	unsigned int val = 0;
	
	/* init config */
	char *device = "default";
    unsigned int channels = 1;
    unsigned int sample_rate = 16000;
    snd_pcm_uframes_t frames = 32; //frames per period
    snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;
    snd_pcm_access_t access = SND_PCM_ACCESS_RW_INTERLEAVED;
    unsigned int sample = 16;
	long recorder_ms = 5000000;

    /* open pcm device for capture */ 
	rc = snd_pcm_open(&handle, device, SND_PCM_STREAM_CAPTURE, 0);
	if (rc < 0)
	{
		fprintf(stderr, "unable to open pcm device: %s\n", snd_strerror(rc));
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

    printf("set pcm hardware parameters successfully\n");

    /* use a buffer large enough to hold one period */
    snd_pcm_hw_params_get_period_size(hw_params, &frames, &dir); 

    size = frames * channels * (sample / 8);
    buffer = (char *)calloc(1, size);
    if (NULL == buffer)
    {
        fprintf(stderr, "unable malloc buffer\n");
        rc = -1;
        goto END;
    }

	/* loop for 5 seconds */
	snd_pcm_hw_params_get_period_time(hw_params, &val, &dir);
	loops = recorder_ms / val;

    fp = fopen(argv[1], "wb");
    if (fp == NULL)
    {
    	fprintf(stderr, "open file: %s failed\n", argv[1]);
        rc = -1;
        goto END;
    }

    printf("recorder %s start\n", argv[1]);

	while (loops > 0)
    {
		loops--;

        rc = snd_pcm_readi(handle, buffer, frames);

    	if (rc == -EPIPE)
    	{
    		fprintf(stderr, "overrun occurred\n");
    		snd_pcm_prepare(handle);
    	}
    	else if (rc < 0)
    	{
    		fprintf(stderr, "error from readi: %s\n", snd_strerror(rc));
    	}
    	else if (rc != (int)frames)
		{
        	fprintf(stderr, "short write, write %d frames\n", rc);
		}
    	
        /* write data */
    	rc = fwrite(buffer, 1, size, fp);
    	if (rc != size)
    	{
    	    fprintf(stderr, "short write file: write %d bytes\n", rc);
    	}
    }

    /* stop a PCM dropping pending frames. */
    snd_pcm_drop(handle);

    printf("recorder end\n");

	rc = 0;
END:
    if (NULL != hw_params)
    {
        snd_pcm_hw_params_free(hw_params);
        hw_params = NULL;
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

    return rc;
}

