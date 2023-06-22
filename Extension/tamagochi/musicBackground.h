#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <math.h>
#include <raylib.h>

#define NUM_WAVES 40


float* readData(char * path);


static init(void);

static addNew(float value);


Texture2D getMusicBackground(Texture2D* t,float time, float* buffer, Color background);
