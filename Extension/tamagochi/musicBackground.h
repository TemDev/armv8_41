#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <math.h>
#include <raylib.h>

#define NUM_WAVES 100


float* readData(char * path);


static init(void);

static addNew(float value);


void getMusicBackground(float time, float* buffer, Color background);
