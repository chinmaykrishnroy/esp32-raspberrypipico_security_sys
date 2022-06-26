'''VERY IMPORTANT TO CONNECT ALL THE COMPONENTS TO THE BOARD BEFORE RUNNING THE CODE OTHERWISE YOU MAY GET SOME ERRORS!!!'''
from machine import Pin, I2C
from machine import Pin, PWM
import utime
import math

dmin = 11
Mid = 1500000
Min = 1300000
Max = 1700000
a = Min  
step = 10000

buzzer = Pin(22, Pin.OUT)
buzzer.value(0)
led = Pin(25, Pin.OUT)
led.value(1)
pwm = PWM(Pin(15))
trig = Pin(20, Pin.OUT)
echo = Pin(21, Pin.IN)

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
        pwm.duty_ns(a)
        a = a + step
        utime.sleep(0.005)
    while(a>=Min and a<= Max):
        pwm.duty_ns(a)
        a = a - step
        utime.sleep(0.005)
