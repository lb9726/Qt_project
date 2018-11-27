#include "alsa.h"
#include "lplay.h"

alsa::alsa(QObject *parent) :
    QObject(parent)
{}

alsa::~alsa()
{
    IDE_TRACE_STR("Relese");
	Alsa_quit();
}

void alsa::slot_Init()
{
    IDE_TRACE_STR("Init");
    Alsa_init();
}

void alsa::slot_Play(QString pPath)
{
    char *tmpPath;
    QByteArray tmpB;
    tmpB.clear();
    tmpB = pPath.toLatin1();
    tmpPath = tmpB.data();
    IDE_TRACE_STR(tmpPath);
    Alsa_play(tmpPath);
    tmpB.clear();
}

void alsa::slot_SetVoiceEnable(bool pEnale)
{
    IDE_TRACE_STR(pEnale);
	Alsa_voiceEnable(pEnale);
}

void alsa::slot_Pause()
{
    IDE_TRACE_STR("Pause");
	Alsa_pause();
}

void alsa::slot_Resume()
{
    IDE_TRACE_STR("Resume");
    Alsa_resume();
}

void alsa::slot_Stop()
{
    IDE_TRACE_STR("Stop");
	Alsa_stop();
}

void alsa::slot_Finished()
{
    if(Alsa_get_finished())
    {
        IDE_TRACE_STR("Finished");
        emit sFinished();
    }
}
