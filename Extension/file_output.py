import light
import temp
import waterlevel

adcout = main_readadc()
temp_c, temp_f = read_temp()
light_off = light_reading()
f = open("sensorReadings.txt", "w")
f.write(str(temp_c) +  "\n" + str(adcout) + "\n" + str(light_off) + "\n")
f.close()
