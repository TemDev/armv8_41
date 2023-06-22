
#include <Python.h>
typedef struct {
  float tempC;
  int waterLevel;
  bool lightOff;
} data;

void fetchData(data *,FILE *);
