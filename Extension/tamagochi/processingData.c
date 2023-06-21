// get data

int temp;  // assume data recieved (degrees C)
int waterlevel;  // assume data recieved [0,1]
int lightLevel;  // assume data recieved [0,1]

bool precipitation
bool heavyPrecipitation;
bool tooHot;
bool tooCold;


void resetEnvironment(void) {
    precipitation = false;
    heavyPrecipitation = false;
    tooHot = false;
    tooCold = false;
}

while (true) {
    // update sensor values
    resetEnvironment();
    if (temp > 20) {
        tooHot = true;
    } else if (temp < 0) {
        tooCold = true;
    }
    if (waterlevel > 0.1) {
        precipitation = true;
    } else if (waterlevel > 0.7) {
        heavyPrecipitation = true;
    }
    // wait 10 seconds
}

/* CHECKS IN MAIN:
sunny = tooHot ^ ¬(prec ^ heavyPrec)
rain = prec ^ ¬tooCold
thunder = heavyPrec ^ ¬tooCold
snow = (prec | heavyPrec) ^ tooCold
*/
