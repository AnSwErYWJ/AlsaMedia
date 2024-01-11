#include "import.h"

/*
 * Display information about the alsa lib
 */
VISIBLE_ATTR void alsa_get_libInfo(void)
{
    printf("\n");

    int i = 0;

    printf("ALSA Library Version: %s\n", SND_LIB_VERSION_STR);

    printf("\n  PCM stream types:\n");
    for (i = 0; i <= SND_PCM_STREAM_LAST; i++)
    {
        printf("    %s\n", snd_pcm_stream_name((snd_pcm_stream_t)i));
    }

    printf("\n  PCM access types:\n");
    for (i = 0; i <= SND_PCM_ACCESS_LAST; i++)
    {
        printf("    %s\n", snd_pcm_access_name((snd_pcm_access_t)i));
    }

    printf("\n  PCM formats:\n");
    for (i = 0; i <= SND_PCM_FORMAT_LAST; i++)
    {
        if (snd_pcm_format_name((snd_pcm_format_t)i) != NULL)
        {
            printf("    %s (%s)\n", snd_pcm_format_name((snd_pcm_format_t)i), snd_pcm_format_description((snd_pcm_format_t)i));
        }
    }

    printf("\n  PCM subformats:\n");
    for (i = 0; i <= SND_PCM_SUBFORMAT_LAST;i++)
    {
        if (snd_pcm_subformat_name((snd_pcm_subformat_t)i) != NULL)
        {
            printf("    %s (%s)\n", snd_pcm_subformat_name((snd_pcm_subformat_t)i), snd_pcm_subformat_description((snd_pcm_subformat_t)i));
        }
    }

    printf("\n  PCM states:\n");
    for (i = 0; i <= SND_PCM_STATE_LAST; i++)
    {
        printf("    %s\n",snd_pcm_state_name((snd_pcm_state_t)i));
    }

    printf("\n");
}

static void _alsa_list_device(snd_pcm_stream_t stream)
{
    int err, idx;
    int card = -1;
    char name[32] = {0};
    snd_ctl_t *handle = NULL;
    snd_ctl_card_info_t *info = NULL;
    snd_pcm_info_t *pcminfo = NULL;
    int dev = -1;
    unsigned int count = 0;

    snd_ctl_card_info_alloca(&info);
    snd_pcm_info_alloca(&pcminfo);

    if (snd_card_next(&card) < 0 || card < 0)
    {
        printf("  no soundcards found\n");
        return;
    }

    while (card >= 0)
    {
        sprintf(name, "hw:%d", card);

        if ((err = snd_ctl_open(&handle, name, 0)) < 0)
        {
            fprintf(stderr, "  control open (%i): %s\n", card, snd_strerror(err));
            goto next_card;
        }

        if ((err = snd_ctl_card_info(handle, info)) < 0)
        {
            fprintf(stderr, "  control hardware info (%i): %s\n", card, snd_strerror(err));
            snd_ctl_close(handle);
            goto next_card;
        }

        dev = -1;

        while (1)
        {
            if ((err = snd_ctl_pcm_next_device(handle, &dev)) < 0)
            {
                fprintf(stderr, "  control next device (%i): %s\n", card, snd_strerror(err));
            }
 
            if (dev < 0)
            {
                break;
            }
 
            snd_pcm_info_set_device(pcminfo, dev);
            snd_pcm_info_set_subdevice(pcminfo, 0);
            snd_pcm_info_set_stream(pcminfo, stream);

            if ((err = snd_ctl_pcm_info(handle, pcminfo)) < 0)
            {
                if (err != -ENOENT)
                {
                    fprintf(stderr, "  control digital audio info (%i): %s\n", card, snd_strerror(err));
                }
                continue;
            }

            printf("  card %i: %s [%s], device %i: %s [%s]\n", card, snd_ctl_card_info_get_id(info), 
                snd_ctl_card_info_get_name(info), dev, snd_pcm_info_get_id(pcminfo), snd_pcm_info_get_name(pcminfo));

            count = snd_pcm_info_get_subdevices_count(pcminfo);
            
            printf("    Subdevices: %i/%i\n", snd_pcm_info_get_subdevices_avail(pcminfo), count);
            
            for (idx = 0; idx < (int)count; idx++)
            {
                snd_pcm_info_set_subdevice(pcminfo, idx);

                if ((err = snd_ctl_pcm_info(handle, pcminfo)) < 0)
                {
                    fprintf(stderr, "    control digital audio playback info (%i): %s\n", card, snd_strerror(err));
                }
                else
                {
                    printf("    Subdevice #%i: %s\n", idx, snd_pcm_info_get_subdevice_name(pcminfo));
                }
            }
        }

        snd_ctl_close(handle);
    
    next_card:
        if (snd_card_next(&card) < 0)
        {
            break;
        }
    }

    printf("\n");
}

/*
 * Display playback and capture devices
 */
VISIBLE_ATTR void alsa_list_device(void)
{
    printf("\n");

    snd_pcm_stream_t stream;
    
    stream = SND_PCM_STREAM_PLAYBACK;
    printf("List of %s Hardware Devices\n", snd_pcm_stream_name(stream));
    _alsa_list_device(stream);

    stream = SND_PCM_STREAM_CAPTURE;
    printf("List of %s Hardware Devices\n", snd_pcm_stream_name(stream));
    _alsa_list_device(stream);    

    printf("\n");
}

int main(void)
{
    alsa_get_libInfo();
    alsa_list_device();

    return 0;
}