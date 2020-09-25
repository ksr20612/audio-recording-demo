#include "playThread.h"
#include "iostream"
#include <alsa/asoundlib.h>
#include "wavReader.h"
#include "QVariant"
using namespace std;

PlayThread::PlayThread(){

    mFrames = 32;
    if(mNumChannel==2){
        mFrames*=2;
    }
    mMaximumDuration = 60;

}

void PlayThread::run(){

    // set variables
//    char name[30];
//    cout << "Plase insert file name" << endl;
//    cin >> name;

    char* extension = ".wav";
    strcat(mFileName, extension);

    WavReader wr;

    if(wr.setVariables(mFileName, mNumChannel, mSamplingSize, mDuration, mSamplingRate)!=0){
        loadFail();
    }else{

        emit showName(mFileName);

        // buffer init
    //    short *buffer = new short[mDuration*mSamplingRate*mNumChannel];
    //    memset(buffer,0,sizeof(short)*mDuration*mSamplingRate*mNumChannel);

        // read data
    //    wr.setData(name, buffer);

        int err;
        snd_pcm_t *playback_handle;
        snd_pcm_hw_params_t *hw_params;

        // open pcm
        if ((err = snd_pcm_open (&playback_handle, "plughw:0,0", SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
            fprintf (stderr, "cannot open audio device (%s)\n",
                 snd_strerror (err));
            exit (1);
        }

        // allocate structure for parameter setting
        if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
            fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
                 snd_strerror (err));
            exit (1);
        }

        // initialize the structure with defaults
        if ((err = snd_pcm_hw_params_any (playback_handle, hw_params)) < 0) {
            fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
                 snd_strerror (err));
            exit (1);
        }

        // determine Access Type (capture =readi / playback =writei)
        if ((err = snd_pcm_hw_params_set_access (playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
            fprintf (stderr, "cannot set access type (%s)\n",
                 snd_strerror (err));
            exit (1);
        }

        // set sampling size
        if ((err = snd_pcm_hw_params_set_format (playback_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
            fprintf (stderr, "cannot set sample format (%s)\n",
                 snd_strerror (err));
            exit (1);
        }

        // set sampling rate
        if ((err = snd_pcm_hw_params_set_rate_near(playback_handle, hw_params, &mSamplingRate, 0)) < 0) {
            fprintf (stderr, "cannot set sample rate (%s)\n",
                 snd_strerror (err));
            exit (1);
        }

        // determine channel count : mono=>1, stereo=>2
        if ((err = snd_pcm_hw_params_set_channels (playback_handle, hw_params, mNumChannel)) < 0) {
            fprintf (stderr, "cannot set channel count (%s)\n",
                 snd_strerror (err));
            exit (1);
        }

        // convey parameters
        if ((err = snd_pcm_hw_params (playback_handle, hw_params)) < 0) {
            fprintf (stderr, "cannot set parameters (%s)\n",
                 snd_strerror (err));
            exit (1);
        }

        // free memory allocated to the structure
        snd_pcm_hw_params_free (hw_params);

        // parepare audio interface
        if ((err = snd_pcm_prepare (playback_handle)) < 0) {
            fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
                 snd_strerror (err));
            exit (1);
        }


       // start playback
        printf("start playback for %d seconds\n", mDuration);
        short *buff = new short[mFrames*mNumChannel];
        memset(buff,0,sizeof(short)*mFrames*mNumChannel);
        FILE* wavFile = fopen(mFileName,"r");
        fseek(wavFile,44,SEEK_SET);
        short *tempBuff = buff;
        while(feof(wavFile)==0){
//            if(mIsStopped==true){
//                while(true) {
//                    usleep(1);
//                    if(mIsStopped==false) break;
//                }
//            }
            fread(tempBuff,mSamplingSize,32,wavFile);
            tempBuff = buff;
            snd_pcm_writei (playback_handle, tempBuff, mFrames);
        }
        usleep(mDuration*1000*1000);
        emit closeName();
        fclose(wavFile);
        cout << "playback completed" << endl;


        // drop buffer
        delete[] buff;

        // drop & close the handler
        snd_pcm_drop(playback_handle);
        snd_pcm_close (playback_handle);
    }
}
