from light import *
from temp import *
from waterlevel import *

def write_file():
    adcout = main_readadc()
    temp_c, temp_f = read_temp()
    light_off = light_reading()
    f = open("sensorReadings.txt", "w")
    f.write(str(temp_c) +  "\n" + str(adcout) + "\n" + str(light_off) + "\n")
    f.close()


def main():
    write_file()

if __name__ == "__main__":
    main()
