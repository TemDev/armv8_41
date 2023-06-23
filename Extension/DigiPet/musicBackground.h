// Header file for musicBackground.c

#include <math.h>
#include <raylib.h>
#include <sndfile.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_WAVES 80

// Reads in the music file
float *readData(char *path);

// Initialises music bars to 0
static init(void);

static addNew(float value);

// Operates on the music bars
void getMusicBackground(float time, float *buffer, Color background);
