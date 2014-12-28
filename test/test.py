#!/usr/bin/python3.4
import time
import dht11_sensor

PIN=7  #GPIO.PIN=4 #wiring pi default pin numbering is 7

print("Start Test:\n")

validData = None

for i in range(1,1000):
    data = dht11_sensor.read(PIN)
    print('Temp={0}*C  Humidity={1}%  Status={2}  Error={3}'.format(data['temperature'], data['humidity'], data['valid'], data['err']))

    if data['valid'] == 1:
        validData = data
        break
   
     
print('Valid Data: Temp={0}*C  Humidity={1}% '.format(validData['temperature'], validData['humidity']))
   

print("\nDone\n")