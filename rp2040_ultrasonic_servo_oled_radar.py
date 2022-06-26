'''VERY IMPORTANT TO CONNECT ALL THE COMPONENTS TO THE BOARD BEFORE RUNNING THE CODE OTHERWISE YOU MAY GET SOME ERRORS!!!'''
from machine import Pin, I2C
from ssd1306 import SSD1306_I2C
import framebuf
from machine import Pin, PWM
import utime
import math

Mid = 1500000
Min = 1300000     # set min value to 500000 to get about -90 degree sweep.
Max = 1700000     # set max value to 2500000 to get about +90 degree sweep.
# set min to 500000 and max to 2500000 to get 180 degree sweep.
a = Min
step = 10000      # quantize the angle motor should make at a time.

# scl clock pin is gpio 13 and data pin is gpio 12.
i2c = I2C(0, scl=Pin(13), sda=Pin(12), freq=400000)
print("I2C Address: "+hex(i2c.scan()[0]).upper())
print("I2C Configuration: "+str(i2c))
oled = SSD1306_I2C(128, 64, i2c)

# set buzzer pin to gpio 22.
buzzer = Pin(22, Pin.OUT)
buzzer.value(0)
led = Pin(25, Pin.OUT)
led.value(1)
pwm = PWM(Pin(4))
# set trigger pin of ultrasonic sensor to gpio 10.
trig = Pin(10, Pin.OUT)
# set trigger pin of ultrasonic sensor to gpio 10.
echo = Pin(11, Pin.IN)

pwm.freq(50)
pwm.duty_ns(a)


def distance_cm():
    trig.low()
    utime.sleep_us(2)
    trig.high()
    utime.sleep_us(4)
    trig.low()
    while echo.value() == 0:
        sLOW = utime.ticks_us()
    while echo.value() == 1:
        sHIGH = utime.ticks_us()
    timePerSignal = sHIGH - sLOW
    distance = (timePerSignal * 0.03493) / 2
    print("Distance", distance, "cm")
    return distance


while True:
    while(a >= (Min - step) and a <= (Max - step)):
        oled.fill(0)
        the_distance = distance_cm()
        while(the_distance <= 15):
            buzzer.value(1)
            led.value(1)
            utime.sleep(0.25)
            buzzer.value(0)
            led.value(0)
            utime.sleep(0.1)
            print("SECURITY BREACH")
            oled.text("SECURITY BREACH", 0, 0)
            oled.text("Target Crossed:", 0, 17)
            oled.text(str(the_distance) + " cm", 0, 26)
            angle = (180/4)*((a/Min)-1)
            oled.text("At the angle:", 0, 34)
            oled.text(str(angle) + " deg", 0, 43)
            oled.show()
        oled.text("Distance: ", 0, 0)
        oled.text(str(the_distance) + " cm", 0, 9)
        angle = (180/4)*((a/Min)-1)
        print("angle is ", angle)
        oled.text("Angle: ", 0, 17)
        oled.text(str(angle) + " deg", 0, 26)
        oled.show()
        pwm.duty_ns(a)
        a = a + step
        print(a)
        utime.sleep(0.001)
    while(a >= Min and a <= Max):
        oled.fill(0)
        the_distance = distance_cm()
        while(the_distance <= 15):
            buzzer.value(1)
            led.value(1)
            utime.sleep(0.25)
            buzzer.value(0)
            led.value(0)
            utime.sleep(0.1)
            print("SECURITY BREACH")
            oled.text("SECURITY BREACH", 0, 0)
            oled.text("Target Crossed:", 0, 17)
            oled.text(str(the_distance) + " cm", 0, 26)
            angle = (180/4)*((a/Min)-1)
            oled.text("At the angle:", 0, 34)
            oled.text(str(angle) + " deg", 0, 43)
            oled.show()
        oled.text("Distance: ", 0, 0)
        oled.text(str(the_distance) + " cm", 0, 9)
        angle = (180/4)*((a/Min)-1)
        print("angle is ", angle)
        oled.text("Angle: ", 0, 17)
        oled.text(str(angle) + " deg", 0, 26)
        oled.show()
        pwm.duty_ns(a)
        a = a - step
        print(a)
        utime.sleep(0.001)
