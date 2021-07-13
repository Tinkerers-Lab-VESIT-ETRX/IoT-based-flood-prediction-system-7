import serial
from firebase import firebase
from time import sleep
from datetime import datetime
import serial.tools.list_ports


ports = serial.tools.list_ports.comports()
for port, desc, hwid in sorted(ports):
        print("{}: {} [{}]".format(port, desc, hwid))


ser = serial.Serial("COM2", 9600)
res =1
i=0
time=datetime.now().strftime("%d-%m-%Y %H:%M:%S")
print(time)

while res:
        
     firebase1 = firebase.FirebaseApplication('https://flood-prediction-system-7dac3-default-rtdb.firebaseio.com/', None)

     for i in range(0,4):
     
             string1=str(ser.readline())
             string1=string1[13:15]
             data =  { 'Date': datetime.now().strftime("%Y-%m-%d"),
                       'Reading':string1,
                       'Time': datetime.now().strftime("%H:%M:%S")}
             result = firebase1.patch('https://flood-prediction-system-7dac3-default-rtdb.firebaseio.com/'+ '/Humidity/' + str(i), data)
             print(result)
             
             string2=str(ser.readline())
             string2=string2[16:18]
             data =  { 'Date': datetime.now().strftime("%Y-%m-%d"),
                       'Reading':string2,
                       'Time': datetime.now().strftime("%H:%M:%S")}
             result = firebase1.patch('https://flood-prediction-system-7dac3-default-rtdb.firebaseio.com/'+ '/Temperature/'+ str(i), data)
             print(result)

     string3=str(ser.readline())
     string3=string3[20:27]
     data =  { 'Date': datetime.now().strftime("%Y-%m-%d"),
               'Reading':string3,
               'Time': datetime.now().strftime("%H:%M:%S")}
     result = firebase1.patch('https://flood-prediction-system-7dac3-default-rtdb.firebaseio.com/'+ '/Weather Forecast/', data)
     print(result)

     string4=str(ser.readline())
     string4=string4[15:18]
     data =  { 'Date': datetime.now().strftime("%Y-%m-%d"),
               'Reading':string4,
               'Time': datetime.now().strftime("%H:%M:%S")}
     result = firebase1.patch('https://flood-prediction-system-7dac3-default-rtdb.firebaseio.com/'+ '/Water level/', data)
     print(result)
     
     res=0
