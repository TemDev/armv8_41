typedef struct {
  float tempC;
  int waterLevel;
  bool lightOff;
} data;

data *fetchData(void);
