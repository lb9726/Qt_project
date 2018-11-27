#include "extern.h"

//>@“Ù¡ø∑∂Œß,0~100
void SetDevVolume(int pVaule)
{
#ifdef LINUX
    snd_mixer_selem_id_t *sid;
    snd_mixer_t *handle;
    snd_mixer_elem_t *elem;
    snd_mixer_selem_channel_id_t chn;
    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, "PCM");
    snd_mixer_open(&handle, 0);
    snd_mixer_attach(handle, "default");
    snd_mixer_selem_register(handle, NULL, NULL);
    snd_mixer_load(handle);
    elem = snd_mixer_find_selem(handle, sid);
    if (elem != NULL)
    {
        if (snd_mixer_selem_is_enumerated(elem))
        {
            int eidx, items;
            items = snd_mixer_selem_set_enum_item(elem, 1, pVaule);
            items = snd_mixer_selem_set_enum_item(elem, 0, pVaule);
            snd_mixer_selem_get_enum_item(elem, chn, &eidx);
            items = snd_mixer_selem_get_enum_items(elem);
            printf("eidx is %d, items is %d\n", eidx, items);
        }
        else
        {
            snd_mixer_selem_set_playback_volume_range(elem, 0, 100);
            snd_mixer_selem_set_playback_volume_all(elem, pVaule);
        }
    }
    snd_mixer_close(handle);
#endif
}
