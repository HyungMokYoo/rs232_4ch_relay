# sudo apt install python3-pip
# sudo pip3 install pyserial

import serial
import time

ser = serial.Serial(
	port='/dev/ttyS0',\
	baudrate=9600,\
	parity=serial.PARITY_NONE,\
	stopbits=serial.STOPBITS_ONE,\
	bytesize=serial.EIGHTBITS,\
	timeout=0)

print(ser.portstr) #연결된 포트 확인.

#ser.write(bytes('hello', encoding='ascii')) #출력방식1
#ser.write(b'hello') #출력방식2

# relay open
ser.write(b'\x55\x56\x00\x00\x00\x01\x01\xAD') #출력방식3
time.sleep(1)
ser.write(b'\x55\x56\x00\x00\x00\x02\x01\xAE') #출력방식3
time.sleep(1)
ser.write(b'\x55\x56\x00\x00\x00\x03\x01\xAF') #출력방식3
time.sleep(1)
ser.write(b'\x55\x56\x00\x00\x00\x04\x01\xB0') #출력방식3
time.sleep(1)

# relay close
ser.write(b'\x55\x56\x00\x00\x00\x01\x02\xAE') #출력방식3
time.sleep(1)
ser.write(b'\x55\x56\x00\x00\x00\x02\x02\xAF') #출력방식3
time.sleep(1)
ser.write(b'\x55\x56\x00\x00\x00\x03\x02\xB0') #출력방식3
time.sleep(1)
ser.write(b'\x55\x56\x00\x00\x00\x04\x02\xB1') #출력방식3
time.sleep(1)

# relay toggle
#ser.write(b'\x55\x56\x00\x00\x00\x01\x03\xAF') #출력방식3
#time.sleep(1)
#ser.write(b'\x55\x56\x00\x00\x00\x02\x03\xB0') #출력방식3
#time.sleep(1)
#ser.write(b'\x55\x56\x00\x00\x00\x03\x03\xB1') #출력방식3
#time.sleep(1)
#ser.write(b'\x55\x56\x00\x00\x00\x04\x03\xB2') #출력방식3
#time.sleep(1)

# relay momentay(200 ms)
ser.write(b'\x55\x56\x00\x00\x00\x01\x04\xB0') #출력방식3
time.sleep(1)
ser.write(b'\x55\x56\x00\x00\x00\x02\x04\xB1') #출력방식3
time.sleep(1)
ser.write(b'\x55\x56\x00\x00\x00\x03\x04\xB2') #출력방식3
time.sleep(1)
ser.write(b'\x55\x56\x00\x00\x00\x04\x04\xB3') #출력방식3
time.sleep(1)

ser.close()
