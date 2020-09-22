#include "wavReader.h"
#include <stdio.h>
#include <iostream>
using namespace std;

void WavReader::setDuration(char *name, int &duration){

    FILE* wavFile = fopen(name,"r");

    int byteRate, dataSize;
    fseek(wavFile,28,SEEK_SET);
    fread(&byteRate,4,1,wavFile);
    fseek(wavFile,8,SEEK_CUR);
    fread(&dataSize,4,1,wavFile);

    duration = dataSize/byteRate;

    fclose(wavFile);
}

void WavReader::setData(char *name, short *buffer){

    FILE* wavFile = fopen(name,"r");

    fseek(wavFile,44,SEEK_SET);
    short *temp = buffer;
    while(feof(wavFile)==0){
        fread(temp,2,1,wavFile);
        temp++;
    }

    fclose(wavFile);
}


void WavReader::setVariables(char *name,int &numChannel, int &sampleSize, int &duration, unsigned int &sampleRate){

    FILE* wavFile = fopen(name,"r");

    fseek(wavFile,22,SEEK_SET);
    fread(&numChannel,2,1,wavFile);
    cout << "channel : " << numChannel << endl;

    fread(&sampleRate,4,1,wavFile);
    cout << "sampleRate : " << sampleRate << endl;

    int byteRate;
    fread(&byteRate,4,1,wavFile);
    sampleSize = byteRate/sampleRate;
    cout << "sampleSize : " << sampleSize << endl;

    int dataSize;
    cout << "dataSize : " << dataSize << endl;
    fseek(wavFile,8,SEEK_CUR);
    fread(&dataSize,4,1,wavFile);
    duration = dataSize/byteRate;
    cout << "duration : " << duration << endl;

    fclose(wavFile);
}
