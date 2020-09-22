#ifndef WAVREADER_H
#define WAVREADER_H
#include "audioVariables.h"

class WavReader{

public:
    void setData(char *name, short *buffer);
    void setDuration(char *name, int &duration);
    void setVariables(char *name,int &numChannel, int &sampleSize, int &duration, unsigned int &sampleRate);
};

#endif // WAVREADER_H
