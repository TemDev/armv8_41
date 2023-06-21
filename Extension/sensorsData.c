#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <Python.h>
#define BUFFER_SIZE 10

typedef struct {
  float tempC;
  int waterLevel;
  bool lightOff;
} data;
  

data *fetchData(void) {
  char filename[] =  "file_output.py";
  Py_Initialize();
  FILE *file;
  file = fopen(filename, "r");
  PyRun_SimpleFile(file, filename );
  Py_Finalize();
  
  char buffer[BUFFER_SIZE];
  data *dataSensors =(data *) malloc(sizeof(data));
  FILE *sensorFile = fopen("sensorReadings.txt", "r");
  
  fgets(buffer, BUFFER_SIZE, sensorFile);
  dataSensors->tempC = atof(buffer);
  
  fgets(buffer, BUFFER_SIZE, sensorFile);
  sscanf(buffer, "%d", &(dataSensors->waterLevel));
  
  fgets(buffer, BUFFER_SIZE, sensorFile);
  dataSensors->lightOff = (!strncmp(buffer, "1", 1)) ? true : false;

  printf("sensorsData.c\n");
  printf("%f\n%d\n%i", dataSensors->tempC, dataSensors->waterLevel, dataSensors->lightOff);

  return dataSensors;
}


int main(void) {
  data* newData = fetchData();
}
