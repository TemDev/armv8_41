#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "sensorsData.h"
#define BUFFER_SIZE 10

void fetchData(data* dataSensors, FILE * sensorFile) {

  
  // take the buffer of the characters
  char* buffer = malloc(BUFFER_SIZE);

  // gets the temperature as a float value
  fgets(buffer, BUFFER_SIZE, sensorFile);
  dataSensors -> tempC = atof(buffer);
  // takes the water level as the integer
  fgets(buffer, BUFFER_SIZE, sensorFile);
  sscanf(buffer, "%d", &(dataSensors->waterLevel));
  
  //takes a true if it is 1 and 0  
  fgets(buffer, BUFFER_SIZE, sensorFile);
  dataSensors->lightOff = (!strncmp(buffer, "1", 1)) ? true : false;

  free(buffer);
}
