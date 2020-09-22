#include "recorder.h"
#include <QObject>
#include <QVariant>
#include <iostream>
#include <alsa/asoundlib.h>
#include <string.h>
#include <stdio.h>
#include <QCoreApplication>
using namespace std;

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
    memset(mBuffer, -1, MAXSIZE*sizeof(short)); // 0으로
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
    //WavWriter wr;
    //wr.writeWav(fileName,mDuration,mBuffer,mSamplingRate,mNumChannel,mSamplingSize);
    mIsRecorded = true;

    // drop handle
    snd_pcm_drop(capture_handle);
    printf("capture completed \n");

    // close handle
    snd_pcm_close (capture_handle);
    printf("handler closed \n");

}
