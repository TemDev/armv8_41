#include <bcm2835.h>
#include <stdio.h>
int presence(uint8_t pin);
void writeBit(uint8_t pin, int b);
void writeByte(uint8_t pin, int byte);
uint8_t readBit(uint8_t pin);
int convert(uint8_t pin);
int readByte(uint8_t pin);
float getTemperature(uint8_t pin);
uint8_t crc8(uint8_t *data, uint8_t len);
int main(int argc, char **argv) {
    if (!bcm2835_init())
        return 1;
    if (presence(RPI_BPLUS_GPIO_J8_07) == 1) {
        printf("No device \n");
    } else {
        printf("Device present \n");
    }
    //fflush(stdout);
    float t;
    for (int i = 0;i<1; i++) {
       printf("smth\n");
	 t = getTemperature(RPI_BPLUS_GPIO_J8_07);
        
        printf("%f\r\n",t);
        //fflush(stdout);
    };
    bcm2835_close();
    return 0;
}
int presence(uint8_t pin) {
    bcm2835_gpio_fsel(pin ,BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(pin,  LOW);
    bcm2835_delayMicroseconds(480);
    bcm2835_gpio_fsel(pin,  BCM2835_GPIO_FSEL_INPT);
    bcm2835_delayMicroseconds(70);
    uint8_t b = bcm2835_gpio_lev(pin);
    bcm2835_delayMicroseconds(410);
    return b;
}
void writeBit(uint8_t pin,  int b) {
    int delay1,  delay2;
    if (b == 1) {
        delay1 = 6;
        delay2 = 64;
    } else {
        delay1 = 60;
        delay2 = 10;
    }
    bcm2835_gpio_fsel(pin,  BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(pin,  LOW);
    bcm2835_delayMicroseconds(delay1);
    bcm2835_gpio_fsel(pin,  BCM2835_GPIO_FSEL_INPT);
    bcm2835_delayMicroseconds(delay2);
}
void writeByte(uint8_t pin,  int byte) {
    int i;
    for (i = 0; i < 8; i++) {
        if (byte & 1) {
            writeBit(pin,  1);
        } else {
            writeBit(pin,  0);
        }
        byte = byte >> 1;
    }
}
uint8_t readBit(uint8_t pin) {
    bcm2835_gpio_fsel(pin,  BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(pin,  LOW);
    bcm2835_delayMicroseconds(8);
    bcm2835_gpio_fsel(pin,  BCM2835_GPIO_FSEL_INPT);
    bcm2835_delayMicroseconds(2);
    uint8_t b = bcm2835_gpio_lev(pin);
    bcm2835_delayMicroseconds(60);
    return b;
}
int convert(uint8_t pin) {
    int i;
    writeByte(pin,  0x44);
    for (i = 0; i < 5000; i++) {
        bcm2835_delayMicroseconds(100000);
        if (readBit(pin) == 1)break;
    }
    return i;
}
float getTemperature(uint8_t pin) {
    if (presence(pin) == 1) return -1000;
    writeByte(pin,  0xCC);
    if (convert(pin) == 5000) return -3000;
    presence(pin);
    writeByte(pin,0xCC);
    writeByte(pin,0xBE);
    int i;
    uint8_t data[9];
    for (i = 0; i < 9; i++) {
        data[i] = readByte(pin);
    }
    uint8_t crc = crc8(data,  9);
    if (crc != 0) return -2000;
    int t1 = data[0];
    int t2 = data[1];
    int16_t temp1 = (t2 << 8 | t1);
    float temp = (float) temp1 / 16;
    return temp;
}
int readByte(uint8_t pin) {
    int byte = 0;
    int i;
    for (i = 0; i < 8; i++) {
        byte = byte | readBit(pin) << i;
    };
    return byte;
}
uint8_t crc8(uint8_t *data,  uint8_t len) {
    uint8_t i;
    uint8_t j;
    uint8_t temp;
    uint8_t databyte;
    uint8_t crc = 0;
    for (i = 0; i < len; i++) {
        databyte = data[i];
        for (j = 0; j < 8; j++) {
            temp = (crc ^ databyte) & 0x01;
            crc >>= 1;
            if (temp)
                crc ^= 0x8C;
            databyte >>= 1;
        }
    }
    return crc;
}
