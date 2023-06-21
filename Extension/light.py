import RPi.GPIO as GPIO
import time
PIN_NUMBER = 27


def init_light():
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(PIN_NUMBER, GPIO.IN)

def light_reading():
    init_light()
    return GPIO.input(PIN_NUMBER)

def main():
    init_light()
    while True:
        print("Light setting is: ", GPIO.input(PIN_NUMBER))
        time.sleep(1)

if __name__ == "__main__":
    main();
