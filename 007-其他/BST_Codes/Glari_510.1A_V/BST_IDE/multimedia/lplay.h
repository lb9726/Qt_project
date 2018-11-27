#ifndef LPLAY_H
#define LPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

extern int Alsa_init(void);

extern void Alsa_pause(void);

extern void Alsa_resume(void);

extern void Alsa_voiceEnable(bool en_voice);

extern void Alsa_stop(void);

extern void Alsa_quit(void);

extern void Alsa_play(const char  *path);

extern bool Alsa_get_finished(void);

#ifdef __cplusplus
}
#endif

#endif
