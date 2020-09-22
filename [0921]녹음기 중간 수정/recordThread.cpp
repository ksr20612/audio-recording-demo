#include "recordThread.h"
#include "iostream"
#include <alsa/asoundlib.h>
#include "wavWriter.h"
using namespace std;

RecordThread::RecordThread(){

    mNumChannel = 1; // mono
    mSamplingSize = 2; // 16bit
    mMaximumDuration = 60;
    mSamplingRate = 8000; // sampling rate
    mFrames = 32;
    if(mNumChannel==2){
        mFrames*=2;
    }
    mBuffer = new short[mMaximumDuration*mSamplingRate*mNumChannel];
    mIsStopped = false;

}

void RecordThread::run(){

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
    memset(mBuffer, 0, MAXSIZE*sizeof(short));
    printf("mBuffer initialized \n");

    // recording
    printf("start recording \n");
    short *tempBuffer = mBuffer;
    long numLoop = 0;
    while(mIsStopped==false){
        numLoop++;
        snd_pcm_readi(capture_handle,tempBuffer,mFrames);
        tempBuffer+=mFrames;
        if(numLoop*mFrames*mSamplingSize>mMaximumDuration*mSamplingRate*mNumChannel) break; // break when exceeding capacity
    }

    // save capture in WAV file
    char fileName[30];
    cout << "Please insert file name" << endl;
    cin >> fileName;
    char *extension = ".wav";
    strcat(fileName, extension);
    WavWriter wr;
    wr.writeWav(fileName,numLoop*mFrames,mBuffer,mSamplingRate,mNumChannel,mSamplingSize);
    // duration => used for calculating total sample numbers
    // hence, number of loops * frames * sampleSize = total sample numbers

    // drop handle
    snd_pcm_drop(capture_handle);
    printf("capture completed \n");

    // close handle
    snd_pcm_close (capture_handle);
    printf("handler closed \n");
}
