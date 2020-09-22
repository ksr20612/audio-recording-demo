#ifndef WAVWRITER_H
#define WAVWRITER_H

#include <stdio.h>

class WavWriter{

public:
    void writeWav(char *name, int duration, short* data, int sampleRate, int numChannel, int sampleSize);
    void writeLittleEndian(unsigned int word, int numBytes, FILE *file);
};

#endif // WAVWRITER_H
