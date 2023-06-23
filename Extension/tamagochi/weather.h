#include <raylib.h>
#include <math.h>
//#include "sensorsData.h"
#include "main.h"
#include "player.h"
#define COLOUR_STEPS 300
#define COLOUR_STEPS2 90

//#include "fruits.h"
#define SCALING_FACTOR 4
#define GRASS_SCALING_FACTOR 13.5
//#define ARRAY_FINISH -1
#define HOT_TRESH 25
#define WET_TRESH 25

void initWeather(void);
bool isRain(environment* env);
bool isHot(environment* env);
