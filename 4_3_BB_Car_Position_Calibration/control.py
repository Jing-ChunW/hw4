import serial
import time
import sys,tty,termios
serdev = '/dev/ttyACM0'
serdev2 = '/dev/ttyUSB0'
s = serial.Serial(serdev, 9600)
s2 = serial.Serial(serdev2, 9600)


while(1):
   line=s.readline() # Read an echo string from mbed terminated with '\n' (RPC reply)
   #print(line)
   #print(chr(line[0]))
   j = 0
   Rx = 0.0
   Ry = 0.0
   Rz = 0.0

   k = 0
   check = 1
   e = 0
   word = [0.0, 0.0, 0.0]
   if chr(line[0]) == "T":
      for i in line:
         #print(chr(i))
         if chr(i) == ":": 
            j = j + 1
            m = k
            
         if j == 4:
            #print(int(i))
            if k-m == 1: Rx = (i-48)*100
            if k-m == 2: Rx = Rx + (i-48)*10
            if k-m == 3: Rx = Rx + (i-48)
            if k-(m+4) == 1: Rx = Rx + (i-48)*0.1
            if k-(m+4) == 2: Rx = Rx + (i-48)*0.01
            if k-(m+4) == 3: Rx = Rx + (i-48)*0.001
            if k-(m+4) == 4: Rx = Rx + (i-48)*0.0001
            if k-(m+4) == 5: Rx = Rx + (i-48)*0.00001
            if k-(m+4) == 6: Rx = Rx + (i-48)*0.000001
         if j == 5:
            if chr(i) == ".": check = 0
            if check == 1:
               if chr(i) != ':':
                  e = k-m-1
                  if e < 3 and e >= 0: word[e] = i
            else: 
               if e == 0: 
                  Ry = (word[0]-48)
                  e = e+3
               if e == 1: 
                  Ry = (word[0]-48)*10+(word[1]-48)
                  e = e+3
               if e == 2: 
                  Ry = (word[0]-48)*100+(word[1]-48)*10+(word[2]-48)
                  e = e+3
               if k-(m+e-1) == 1: Ry = Ry + (i-48)*0.1
               if k-(m+e-1) == 2: Ry = Ry + (i-48)*0.01
               if k-(m+e-1) == 3: Ry = Ry + (i-48)*0.001
               if k-(m+e-1) == 4: Ry = Ry + (i-48)*0.0001
               if k-(m+e-1) == 5: Ry = Ry + (i-48)*0.00001
               if k-(m+e-1) == 6: Ry = Ry + (i-48)*0.000001
         if j == 6:
            if k-m == 1: Rz = (i-48)*100
            if k-m == 2: Rz = Rz + (i-48)*10
            if k-m == 3: Rz = Rz + (i-48)
            if k-(m+4) == 1: Rz = Rz + (i-48)*0.1
            if k-(m+4) == 2: Rz = Rz + (i-48)*0.01
            if k-(m+4) == 3: Rz = Rz + (i-48)*0.001
            if k-(m+4) == 4: Rz = Rz + (i-48)*0.0001
            if k-(m+4) == 5: Rz = Rz + (i-48)*0.00001
            if k-(m+4) == 6: Rz = Rz + (i-48)*0.000001

         k = k + 1     

      #print(chr(line[4])+chr(line[5])+chr(line[6])+chr(line[7]))
      
      Rx = round(Rx, 6)
      Ry = round(Ry, 6)
      Rz = round(Rz, 6)
      pr = 1
      print(str(Rx)+","+str(Ry)+","+str(Rz))
      '''if Cx > 80:
         s2.write("/turn/run -100 \n".encode())'''
      if 0 < Ry and Ry < 180.0:
         s2.write("/myled1/write 1\n".encode())
         s2.write("/turn/run -20 0.3\n".encode())
         #s2.write(("/turn/run -%d -0.7 \n"%(3*(Ry-0))).encode())
      elif 360 > Ry and Ry > 180.0:
         s2.write("/myled2/write 1\n".encode())
         s2.write("/turn/run -20 0.3 \n".encode())
         #s2.write(("/turn/run -%d 0.7 \n"%(3*(365-Ry))).encode())
      while pr == 1:
         line = s.readline()
         if chr(line[0]) == "P":
            print(line)
            pr = 0
      time.sleep(1)
      s2.write("/myled1/write 0\n".encode())
      s2.write("/myled2/write 0\n".encode())
      s2.write("/stop/run \n".encode())



   #s2.write("/turn/run 100 -0.3 \n".encode())

   #time.sleep(1)

