/*************************************************************************
	> File Name: public.c
	> Author: weijie.yuan
	> Mail: yuanweijie1993@gmail.com
	> Created Time: Wed 11 Jan 2017 02:43:37 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include "public.h"

/* Memory alloc and free */
void *acalloc(size_t n,size_t size)
{
    void *mem;
    mem = (void *)calloc(n,size);
    if(mem == NULL)
    {
        fprintf(stderr,"CALLOC failed!\n");
        exit(EXIT_FAILURE);
    }

    return mem;
}

void afree(void *mem)
{
    if (mem != NULL)
    {
        free(mem);
        mem = NULL;
    }
}

/*
 * Display information about the alsa lib
 */
void display_alsalib()
{
    int i;

    printf("ALSA library version: %s\n",SND_LIB_VERSION_STR);

    printf("\nPCM stream types:\n");
    for (i = 0; i <= SND_PCM_STREAM_LAST; i++)
        printf(" %s\n",snd_pcm_stream_name((snd_pcm_stream_t)i));

    printf("\nPCM access types:\n");
    for (i = 0; i <= SND_PCM_ACCESS_LAST; i++)
        printf(" %s\n",snd_pcm_access_name((snd_pcm_access_t)i));

    printf("\nPCM formats:\n");
    for (i = 0; i <= SND_PCM_FORMAT_LAST; i++)
    {
        if (snd_pcm_format_name((snd_pcm_format_t)i)!= NULL)
            printf(" %s (%s)\n",snd_pcm_format_name((snd_pcm_format_t)i),snd_pcm_format_description((snd_pcm_format_t)i));
    }

    printf("\nPCM subformats:\n");
    for (i = 0; i <= SND_PCM_SUBFORMAT_LAST;i++)
        printf(" %s (%s)\n",snd_pcm_subformat_name((snd_pcm_subformat_t)i),snd_pcm_subformat_description((snd_pcm_subformat_t)i));

    printf("\nPCM states:\n");
    for (i = 0; i <= SND_PCM_STATE_LAST; i++)
        printf(" %s\n",snd_pcm_state_name((snd_pcm_state_t)i));
}

/*
 * Display information about the PCM interface
 */
void display_info(snd_pcm_t *handle,snd_pcm_hw_params_t *params)
{
    unsigned int i, j;
    int dir;
    snd_pcm_uframes_t frames;

    printf("PCM handle name = '%s'\n", snd_pcm_name(handle));

    printf("PCM state = %s\n", snd_pcm_state_name(snd_pcm_state(handle)));

    snd_pcm_hw_params_get_access(params,(snd_pcm_access_t *) &i);
    printf("access type = %s\n",snd_pcm_access_name((snd_pcm_access_t)i));

    snd_pcm_hw_params_get_format(params, (snd_pcm_format_t *)&i);
    printf("format = '%s' (%s)\n",snd_pcm_format_name((snd_pcm_format_t)i),snd_pcm_format_description((snd_pcm_format_t)i));

    snd_pcm_hw_params_get_subformat(params,(snd_pcm_subformat_t *)&i);
    printf("subformat = '%s' (%s)\n",snd_pcm_subformat_name((snd_pcm_subformat_t)i),snd_pcm_subformat_description((snd_pcm_subformat_t)i));

    snd_pcm_hw_params_get_channels(params, &i);
    printf("channels = %d\n", i);

    snd_pcm_hw_params_get_rate(params, &i, &dir);
    printf("rate = %d bps\n", i);

    snd_pcm_hw_params_get_period_time(params,&i, &dir);
    printf("period time = %d us\n", i);

    snd_pcm_hw_params_get_period_size(params,&frames, &dir);
    printf("period size = %d frames\n", (int)frames);

    snd_pcm_hw_params_get_buffer_time(params,&i, &dir);
    printf("buffer time = %d us\n", i);

    snd_pcm_hw_params_get_buffer_size(params,(snd_pcm_uframes_t *) &i);
    printf("buffer size = %d frames\n", i);

    snd_pcm_hw_params_get_periods(params, &i, &dir);
    printf("periods per buffer = %d frames\n", i);

    snd_pcm_hw_params_get_rate_numden(params,&i, &j);
    printf("exact rate = %d/%d bps\n", i, j);

    i = snd_pcm_hw_params_get_sbits(params);
    printf("significant bits = %d\n", i);

    snd_pcm_hw_params_get_tick_time(params, &i, &dir);
    printf("tick time = %d us\n", i);

    i = snd_pcm_hw_params_is_batch(params);
    printf("is batch = %d\n", i);

    i = snd_pcm_hw_params_is_block_transfer(params);
    printf("is block transfer = %d\n", i);

    i = snd_pcm_hw_params_is_double(params);
    printf("is double = %d\n", i);

    i = snd_pcm_hw_params_is_half_duplex(params);
    printf("is half duplex = %d\n", i);

    i = snd_pcm_hw_params_is_joint_duplex(params);
    printf("is joint duplex = %d\n", i);

    i = snd_pcm_hw_params_can_overrange(params);
    printf("can overrange = %d\n", i);

    i = snd_pcm_hw_params_can_mmap_sample_resolution(params);
    printf("can mmap = %d\n", i);

    i = snd_pcm_hw_params_can_pause(params);
    printf("can pause = %d\n", i);

    i = snd_pcm_hw_params_can_resume(params);
    printf("can resume = %d\n", i);

    i = snd_pcm_hw_params_can_sync_start(params);
    printf("can sync start = %d\n", i);
}
