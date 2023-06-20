from light import light_reading
from temp import read_temp
from waterlevel import main_readadc

def write_file():
    adcout = main_readadc()
    temp_c, temp_f = read_temp()
    light_off = light_reading()
    f = open("sensorReadings.txt", "w")
    f.write(temp_c)
    f.write(adcout)
    f.wrtie(light_off)
    f.close()


def main():
    write_file()

if __name__ == "__main__":
    main()
