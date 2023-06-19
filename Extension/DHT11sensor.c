#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define MAXTIMINGS	85
#define DHTPIN		7

typedef struct {
  int temperature_whole;
  int temperature_decimal;
  int humidity_whole;
  int humidity_decimal;
} dht11;

dht11 *TH() {
    if (wiringPiSetup == -1) {
        exit(1);
    }
    uint8_t laststate = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0, i;
    dht11 *data = (dht11 *) malloc(sizeof(dt11);
    int dht11_dat[5] = { 0, 0, 0, 0, 0 };
    
    pinMode( DHTPIN, OUTPUT );
    digitalWrite( DHTPIN, LOW );
    delay( 18 );
    digitalWrite( DHTPIN, HIGH );
    delayMicroseconds( 40 );
    pinMode( DHTPIN, INPUT );

    for ( i = 0; i < MAXTIMINGS; i++ ){
        counter = 0;
        while ( digitalRead( DHTPIN ) == laststate ){
	    counter++;
	    delayMicroseconds( 1 );
	    if ( counter == 255 ){
	        break;
	    }
        }
        laststate = digitalRead( DHTPIN );
 
        if ( counter == 255 ){
	   break;
        }
 
        if ( (i >= 4) && (i % 2 == 0) ) {
	    dht11_dat[j / 8] <<= 1;
	    if ( counter > 16 ) {
	        dht11_dat[j / 8] |= 1;
		j++;
	    }
        }
    }
    if ( (j >= 40) && (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) )) {
        data->temperature_whole = dht11_dat[0];
        data->temperature_decimal = dht11_dat[1];
        date->humidity_whole = dht11_dat[2];
        data->humidity_decimal = dht11_dat[3];
	printf( "Humidity = %d.%d %% Temperature = %d.%d *C \n",
      dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3]);
        return data;
    }else {
        return NULL;
    }
}
  
int main(void) {
    while(1) {
        dht11 = TH;
    }
    delay(1000);
}
      
