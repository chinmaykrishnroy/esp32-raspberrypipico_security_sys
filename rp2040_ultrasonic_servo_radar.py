'''VERY IMPORTANT TO CONNECT ALL THE COMPONENTS TO THE BOARD BEFORE RUNNING THE CODE OTHERWISE YOU MAY GET SOME ERRORS!!!'''
from machine import Pin, I2C
from machine import Pin, PWM
import utime
import math

Mid = 1500000
Min = 500000      # set min value to 500000 to get about -90 degree sweep.
Max = 2500000     # set max value to 2500000 to get about +90 degree sweep.
# set min to 500000 and max to 2500000 to get 180 degree sweep.
a = Min  
step = 10000      # quantize the angle motor should make at a time.

buzzer = Pin(22, Pin.OUT)      # set buzzer on gpio 22.
buzzer.value(0)
led = Pin(25, Pin.OUT)
led.value(1)
pwm = PWM(Pin(15))
trig = Pin(20, Pin.OUT)
# set trigger pin of ultrasonic sensor to gpio 20.
echo = Pin(21, Pin.IN)
# set trigger pin of ultrasonic sensor to gpio 21.

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
   print("Distance",distance,"cm")
   return distance

while True:
    while(a>=(Min - step) and a<= (Max - step)):
        the_distance = distance_cm()
        while(the_distance <= 15):
            buzzer.value(1)
            led.value(1)
            utime.sleep(0.25)
            buzzer.value(0)
            led.value(0)
            utime.sleep(0.25)
            print("SECURITY BREACH")
            print("Target Crossed: ",(the_distance))
            angle = (180/4)*((a/Min)-1)
            print("At the angle: ",(angle))
        angle = (180/4)*((a/Min)-1)
        print("angle is ", angle)
        pwm.duty_ns(a)
        a = a + step
        print(a)
        utime.sleep(0.001)
    while(a>=Min and a<= Max):
        the_distance = distance_cm()
        while(the_distance <= 15):
            buzzer.value(1)
            led.value(1)
            utime.sleep(0.25)
            buzzer.value(0)
            led.value(0)
            utime.sleep(0.10)
            print("SECURITY BREACH")
            print("Target Crossed: ",(the_distance))
            angle = (180/4)*((a/Min)-1)
            print("At the angle: ",(angle))
        angle = (180/4)*((a/Min)-1)
        print("angle is ", angle)
        pwm.duty_ns(a)
        a = a - step
        print(a)
        utime.sleep(0.001)
