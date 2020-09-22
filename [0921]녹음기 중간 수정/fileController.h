#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H
#include <stdio.h>
#include "recorder.h"

class FileController{

short* mData = new short[60*8000*1];
int mTime;

public:
    void readWav(char *name);
    static void writeWav(char *filename, unsigned long num_samples, short* data, int s_rate);
    static void writeLittleEndian(unsigned int word, int numBytes, FILE *wav_file);
    short* getmData();
    int getmTime();

};

#endif // FILECONTROLLER_H
