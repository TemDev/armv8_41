import RPi.GPIO as GPIO
import time
PIN_NUMBER = 27
GPIO.setmode(GPIO.BCM)
GPIO.setup(PIN_NUMBER, GPIO.IN)


def light_reading():
    return GPIO.input(PIN_NUMBER);

def main():
    while True:
        print("Light setting is: ", GPIO.input(PIN_NUMBER))
        time.sleep(1)

if __name__ == "__main__":
    main();
