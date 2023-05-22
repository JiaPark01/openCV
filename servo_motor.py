import smbus
from Motor_Lib import PWM
from time import sleep

SERVO1 = 3
SERVO2 = 4

i2c_bus = smbus.SMBus(1)
pwm = PWM(i2c_bus)
pwm.set_frequency(60)

try:
    while True:
        for i in range(540, -1, -5):
            pwm.set_duty(SERVO1, i)
            pwm.set_duty(SERVO2, i)
            sleep(0.1)
except KeyboardInterrupt:
    pass

i2c_bus.close()
