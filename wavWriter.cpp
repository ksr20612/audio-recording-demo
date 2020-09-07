#include <stdio.h>
#include <iostream>
#include <assert.h>
#include "wavWriter.h"
using namespace std;

void WavWriter::writeLittleEndian(unsigned int value, int numBytes, FILE *wavFile){

    unsigned int buf;
    while(numBytes>0){
        buf = value & 0xff;
        fwrite(&buf,1,1, wavFile);
        numBytes--;
        value >>= 8; // move 1 byte
    }
}

void WavWriter::writeWav(char *filename, int duration, short *data, int samplingRate, int numChannel, int sampleSize){

    FILE* wavFile = fopen(filename, "w");
    assert(wavFile); // check if file is ok

    unsigned long numSamples = duration*samplingRate;

    fwrite("RIFF", 1, 4, wavFile);                                                  // Chunk ID
    writeLittleEndian(36 + sampleSize*numSamples*numChannel, 4, wavFile);           // Chunk Size ( (header-self=36) + data size(bps * nos)
    fwrite("WAVE", 1, 4, wavFile);                                                  // Format
    fwrite("fmt ", 1, 4, wavFile);                                                  // Chunk ID
    writeLittleEndian(16, 4, wavFile);                                              // Chunk Size : fixed
    writeLittleEndian(1, 2, wavFile);                                               // Audio Format : fixed
    writeLittleEndian(numChannel, 2, wavFile);                                      // number of channels : fixed 1
    writeLittleEndian(samplingRate, 4, wavFile);                                    // sample rate
    writeLittleEndian(samplingRate*numChannel*sampleSize, 4, wavFile);              // byte rate(bytes per sec)
    writeLittleEndian(numChannel*sampleSize, 2, wavFile);                           // block align
    writeLittleEndian(8*sampleSize, 2, wavFile);                                    // bit per sample
    fwrite("data", 1, 4, wavFile);                                                  // chunk ID
    writeLittleEndian(sampleSize*numSamples*numChannel, 4, wavFile);                // Chunk Size(data size)
    for (unsigned long i=0; i<numSamples; i++){
        writeLittleEndian((unsigned int)(data[i]),sampleSize, wavFile);             // Audio data
    }
    cout << "===== FILE SAVED : " << filename << " =====" << endl;
    fclose(wavFile);
}


