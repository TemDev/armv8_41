#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "sensorsData.h"
#define BUFFER_SIZE 10

void fetchData(data* dataSensors, FILE * sensorFile) {
  
  
  char* buffer = malloc(BUFFER_SIZE);

  
  fgets(buffer, BUFFER_SIZE, sensorFile);
  dataSensors -> tempC = atof(buffer);
  
  fgets(buffer, BUFFER_SIZE, sensorFile);
  sscanf(buffer, "%d", &(dataSensors->waterLevel));
  
  fgets(buffer, BUFFER_SIZE, sensorFile);
  dataSensors->lightOff = (!strncmp(buffer, "1", 1)) ? true : false;

  printf("sensorsData.c\n");
  printf("%f\n%d\n%i", dataSensors->tempC, dataSensors->waterLevel, dataSensors->lightOff);
  free(buffer);
}

// #include <stdio.h>
// #include <stdint.h>
// #include <stdbool.h>
// #include <stdlib.h>
// #include <string.h>
// #include <Python.h>
// #include "sensorsData.h"
// #define BUFFER_SIZE 10

// data fetchData(void) {
  
//   // data dataSensors;
//   // //printf("Running some stuff\n");
//   // // custom stuff

//   // PyObject* module = PyImport_ImportModule("file_output");
//   //   if (!module) {
//   //       PyErr_Print();
//   //   }


//   // PyObject* result = PyObject_CallMethod(module, "main_readadc", NULL);
//   //   if (!result) {
//   //       PyErr_Print();
//   //       Py_DECREF(module);
//   //   }

//   //   // Extract the float value from the result
//   //   dataSensors.waterLevel = PyLong_AsLong(result);

//   //   result = PyObject_CallMethod(module, "read_temp", NULL);
//   //   if (!result) {
//   //       PyErr_Print();
//   //       Py_DECREF(module);
//   //   }

//   //   dataSensors.tempC = PyFloat_AsDouble(result);

//   //   result = PyObject_CallMethod(module, "light_reading", NULL);
//   //   if (!result) {
//   //       PyErr_Print();
//   //       Py_DECREF(module);
//   //   }

//   //   int temp = PyLong_AsLong(result);
//   //   dataSensors.lightOff = (temp == 1) ? true : false;

//   //   printf("%f\n%d\n%i", dataSensors.tempC, dataSensors.waterLevel, dataSensors.lightOff);

//   //   // Do something with the float value
  
    
    

// OLD code from tem


// // char filename[] =  "file_output.py";

// //   FILE *file;
// //   file = fopen(filename, "r");
// //   PyRun_SimpleFile(file, filename );
// //   //fclose(file);
  
  
// //   char buffer[BUFFER_SIZE];
// //   data temp;
// //   data* dataSensors = &temp;
// //   FILE *sensorFile = fopen("sensorReadings.txt", "r");
  
// //   fgets(buffer, BUFFER_SIZE, sensorFile);
// //   dataSensors->tempC = atof(buffer);
  
// //   fgets(buffer, BUFFER_SIZE, sensorFile);
// //   sscanf(buffer, "%d", &(dataSensors->waterLevel));
  
// //   fgets(buffer, BUFFER_SIZE, sensorFile);

// //   dataSensors->lightOff = (!strncmp(buffer, "1", 1)) ? true : false;

// //   //printf("sensorsData.c\n");
// //   //printf("%f\n%d\n%i", dataSensors->tempC, dataSensors->waterLevel, dataSensors->lightOff);
// //   fclose(sensorFile);
  

// //   return *dataSensors;
//   return (data) {0,0,0};
// }
