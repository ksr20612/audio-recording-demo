#include <stdio.h>
#include <iostream>
#include "fileController.h"
#include <assert.h>
using namespace std;

void FileController::readWav(char *name){

    FILE* wavFile = fopen(name, "r");

    // calculate duration
    int byteRate, dataSize;
    fseek(wavFile,28,SEEK_SET);
    fread(&byteRate,4,1,wavFile);
    fseek(wavFile,8,SEEK_CUR);
    fread(&dataSize,4,1,wavFile);
    mTime = dataSize/byteRate;
    cout << "duration : " << mTime << endl;

    // read data
    short *temp = mData;
    while(feof(wavFile)==0){
        fread(temp,2,1,wavFile);
        temp++;
    }
}

void FileController::writeLittleEndian(unsigned int value, int numBytes, FILE *wavFile){

    unsigned int buf;
    while(numBytes>0){
        buf = value & 0xff;
        fwrite(&buf,1,1, wavFile);
        numBytes--;
        value >>= 8; // move 1 byte
    }
}

void FileController::writeWav(char *filename, unsigned long numSamples, short *data, int samplingRate){

    //cout << "data : " << data << endl;

    FILE* wavFile = fopen(filename, "w");

    unsigned int numChannel = 1;
    unsigned int sampleSize = 2;
    unsigned int byteRate = samplingRate*numChannel*sampleSize; // total bytes per second

    assert(wavFile); // check if file is ok

    fwrite("RIFF", 1, 4, wavFile);                                                  // Chunk ID
    writeLittleEndian(36 + sampleSize*numSamples*numChannel, 4, wavFile);           // Chunk Size ( (header-self=36) + data size(bps * nos)
    fwrite("WAVE", 1, 4, wavFile);                                                  // Format
    fwrite("fmt ", 1, 4, wavFile);                                                  // Chunk ID
    writeLittleEndian(16, 4, wavFile);                                              // Chunk Size : fixed
    writeLittleEndian(1, 2, wavFile);                                               // Audio Format : fixed
    writeLittleEndian(numChannel, 2, wavFile);                                      // number of channels : fixed 1
    writeLittleEndian(samplingRate, 4, wavFile);                                    // sample rate
    writeLittleEndian(byteRate, 4, wavFile);                                        // byte rate
    writeLittleEndian(numChannel*sampleSize, 2, wavFile);                           // block align
    writeLittleEndian(8*sampleSize, 2, wavFile);                                    // bit per sample
    fwrite("data", 1, 4, wavFile);                                                  // chunk ID
    writeLittleEndian(sampleSize* numSamples*numChannel, 4, wavFile);               // Chunk Size

    for (unsigned long i=0; i<numSamples; i++){
        writeLittleEndian((unsigned int)(data[i]),sampleSize, wavFile);
    }

    cout << "===== WAV FILE SAVED : " << filename << " =====" << endl;

    fclose(wavFile);
}

short* FileController::getmData(){ return mData; }
int FileController::getmTime(){ return mTime; }
