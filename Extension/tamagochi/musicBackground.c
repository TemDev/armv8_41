#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <math.h>
#include "player.h"
#include "musicBackground.h"


static float waves[NUM_WAVES];
static sampleRate;
static Image img;

float* readData(char * path) {
    SNDFILE *sndfile;
    SF_INFO sfInfo;
    sndfile = sf_open(path, SFM_READ, &sfInfo);
    if (sndfile == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }
    sampleRate = sfInfo.samplerate;
    int numChannels = sfInfo.channels;
    float *buffer  = malloc(sizeof(float) * numChannels * sampleRate);

    int seconds = sfInfo.frames / sampleRate;

    
    for (int sec = 0; sec < seconds; sec++) {
        sf_readf_float(sndfile, buffer, sampleRate);  
    }
    init();
    return buffer;

}



static init(void) {
     for (int i = 0; i < NUM_WAVES; i++) {
        waves[i] =  0;
     }
  //   img = GenImageColor(SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
}

static addNew(float value) {
     for (int i = NUM_WAVES - 1; i > 0; i--) {
        waves[i] =  waves[i - 1];
     }
     printf("new float value%f", value);
     waves[0] = value;
}


void getMusicBackground(float time, float* buffer, Color background) {
    


    float average = 0;
    int start = (time * sampleRate);

    for (int i = (time * sampleRate); i < (time  + (float)(1.0 / NUM_WAVES)) * sampleRate; i ++){
        average += buffer[i];
    }

    average = (average * NUM_WAVES) / sampleRate;
    addNew(average);
        
    
    float width = SCREEN_WIDTH / NUM_WAVES;
    for (int i = 0; i < NUM_WAVES; i++) {
        DrawRectangle(i * width, SCREEN_HEIGHT/2, width,waves[i] * 400, RED);
    }

}
