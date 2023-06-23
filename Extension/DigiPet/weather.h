// Initialises the weather and contains function that check for certain weather conditions
//#include "sensorsData.h"
#include "player.h"
#define COLOUR_STEPS 300
#define COLOUR_STEPS2 90

//#include "fruits.h"
#define SCALING_FACTOR 4
#define GRASS_SCALING_FACTOR 13.5
//#define ARRAY_FINISH -1
#define HOT_TRESH 25
#define WET_TRESH 25

// Initialises the weather by loading and scaling all the images
void initWeather(void);

// Returns whether the waterLevel is greater than the threshold for wetness
bool isRain(environment *env);

// Returns whether temperature is greater than the threshold for hotness
bool isHot(environment *env);
