#include "recorder.h"
#include "wavWriter.h"
#include "wavReader.h"
#include <iostream>
#include <alsa/asoundlib.h>
#include <string.h>
#include <stdio.h>
#include <QCoreApplication>
#include <thread>
using namespace std;

void Recorder::connectQmlCpp(){

    QObject::connect(mMainView, SIGNAL(startRecord(QVariant)),this,SLOT(startRecording(QVariant))); // qml -> cpp
    QObject::connect(mMainView, SIGNAL(startPlay(QVariant)),this,SLOT(startPlaying(QVariant))); // qml -> cpp

}

void Recorder::setWindow(QQuickWindow* Window){

    mMainView = Window;
    connectQmlCpp();
}

void Recorder::startRecording(QVariant intData){

    if(intData==1) cout << "YOU CAN START RECORDING" << endl;

    do{
        cout << "insert duration" << endl;
        cin >> mDuration;
        if(mDuration>mMaximumDuration) printf("insert duration again(maximum : %d sec)\n",mMaximumDuration);
    }while(mDuration>mMaximumDuration);

    int err;
    snd_pcm_t *capture_handle;
    snd_pcm_hw_params_t *hw_params;

    // open pcm
    if ((err = snd_pcm_open (&capture_handle, "plughw:0,0", SND_PCM_STREAM_CAPTURE, 0)) < 0) {
      fprintf (stderr, "fail to open pcm. (%s)\n",
               snd_strerror (err));
      exit (1);
    }
    fprintf(stdout, "pcm openned \n");

    // allocate structure for parameter setting
    if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
      fprintf (stderr, "fail to allocate structure for parameter setting. (%s)\n",
               snd_strerror (err));
      exit (1);
    }
    fprintf(stdout, "structure allocated\n");

    // parameter initialization with defaults
    if ((err = snd_pcm_hw_params_any (capture_handle, hw_params)) < 0) {
      fprintf (stderr, "fail to initialize parameters (%s)\n",
               snd_strerror (err));
      exit (1);
    }
    fprintf(stdout, "parameter initialized\n");

    // access type setting (audio capture =readi / audio playback =writei)
    if ((err = snd_pcm_hw_params_set_access (capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
      fprintf (stderr, "fail to set access type (%s)\n",
               snd_strerror (err));
      exit (1);
    }
    fprintf(stdout, "Access Type setted\n");

    // sampling size setting
    if ((err = snd_pcm_hw_params_set_format (capture_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
      fprintf (stderr, "fail to set sampling rate (%s)\n",
               snd_strerror (err));
      exit (1);
    }
    fprintf(stdout, "sampling rate setted\n");

    // sampling rate setting
    if ((err = snd_pcm_hw_params_set_rate_near (capture_handle, hw_params, &mSamplingRate, 0)) < 0) {
      fprintf (stderr, "fail to set sampling freq (%s)\n",
               snd_strerror (err));
      exit (1);
    }
    fprintf(stdout, "sampling freq setted \n");

    // channel selecting(mono-1- or stereo-2-)
    int mono = 1;
    if ((err = snd_pcm_hw_params_set_channels (capture_handle, hw_params, mono)) < 0) {
      fprintf (stderr, "fail to select appropriate channel (%s)\n",
               snd_strerror (err));
      exit (1);
    }
    fprintf(stdout, "channel determined \n");

    // conveying parameters to the handler
    if ((err = snd_pcm_hw_params (capture_handle, hw_params)) < 0) {
      fprintf (stderr, "fail to deliever parameters (%s)\n",
               snd_strerror (err));
      exit (1);
    }
    fprintf(stdout, "parameters delievered \n");

    // free memory allocated
    snd_pcm_hw_params_free (hw_params);
    fprintf(stdout, "free structure \n");

    // prepare the handle
    if ((err = snd_pcm_prepare (capture_handle)) < 0) {
      fprintf (stderr, "fail to prepare sound interface (%s)\n",
               snd_strerror (err));
      exit (1);
    }
    fprintf(stdout, "interface prepared \n");

    // initialize mBuffer
    #define MAXSIZE mMaximumDuration*mSamplingRate*mNumChannel
    memset(mBuffer, -1, MAXSIZE*sizeof(short));
    printf("mBuffer initialized \n");

    // recording
    printf("start recording for %d seconds\n", mDuration);
    long loops = mDuration * mSamplingRate / mFrames;
    short *tempBuffer = mBuffer;
    while(loops>0){
        loops--;
        snd_pcm_readi(capture_handle,tempBuffer,mFrames);
        tempBuffer+=mFrames;
    }
    cout << "capture completed" << endl;

    // save capture in WAV file
    char fileName[30];
    cout << "Please insert file name" << endl;
    cin >> fileName;
    char *extension = ".wav";
    strcat(fileName, extension);
    WavWriter wr;
    wr.writeWav(fileName,mDuration,mBuffer,mSamplingRate,mNumChannel,mSamplingSize);
    mIsRecorded = true;

    // drop handle
    snd_pcm_drop(capture_handle);
    printf("capture completed \n");

    // close handle
    snd_pcm_close (capture_handle);
    printf("handler closed \n");

}

void Recorder::startPlaying(QVariant intdata){

    if(intdata == 1) cout << "YOU CAN START PLAYING" << endl;

    if(mIsRecorded==false) {

        char name[30];
        cout << "NO DATA IN BUFFER : Plase insert file name" << endl;
        cin >> name;
        char* extension = ".wav";
        strcat(name, extension);

        WavReader wr;
        wr.setDuration(name, mDuration);
        wr.setData(name, mBuffer);

    }

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
    int mono = 1;
    if ((err = snd_pcm_hw_params_set_channels (playback_handle, hw_params, mono)) < 0) {
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
    long loops = mDuration * mSamplingRate / mFrames;
    short* tempBuffer = mBuffer;
    while(loops>0){
        loops--;
        snd_pcm_writei (playback_handle, tempBuffer, mFrames);
        tempBuffer+=mFrames;
    }
    cout << "playback completed" << endl;

    // drop & close the handler
    snd_pcm_drop(playback_handle);
    snd_pcm_close (playback_handle);

}
