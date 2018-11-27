#ifndef __LMAD_H   
#define __LMAD_H  

extern int                      decode(unsigned char const *start, unsigned long length);
extern SNDPCMContainer_t        playback;
extern WAVContainer_t           wav;
extern bool                     m_VoiceEnable;
extern bool                     m_Stop;
extern bool                     m_Finished;
extern const char               *m_MusicPath;
#endif /* #ifndef __LMAD_H */  
