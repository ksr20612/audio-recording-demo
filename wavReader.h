#ifndef WAVREADER_H
#define WAVREADER_H

class WavReader{

public:
    void setData(char *name, short *buffer);
    void setDuration(char *name, int &duration);
};

#endif // WAVREADER_H
