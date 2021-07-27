#ifndef AUDIOCLIP_H
#define AUDIOCLIP_H

#include "ClipBase.h"

class AudioClip : public ClipBase
{
public:
    AudioClip(QUrl url);
};

#endif // AUDIOCLIP_H
