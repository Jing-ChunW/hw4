import serial
import time
import sys,tty,termios
serdev = '/dev/ttyACM0'
serdev2 = '/dev/ttyUSB0'
s = serial.Serial(serdev, 9600)
s2 = serial.Serial(serdev2, 9600)

while(1):
    line = s.readline()
    
    k = 0
    j = 0
    word = [0, 0, 0]
    word2 = [0, 0]
    word3 = [0, 0, 0]
    word4 = [0, 0]
    x1 = 0
    x2 = 0
    y1 = 0
    y2 = 0
    w1 = 3
    w2 = 2
    w3 = 3
    w4 = 2
    if chr(line[0]) == "x":
        print(line)
        for i in line:
            if chr(i) == ":":
                j = j + 1
                m = k
            if j == 1:
                check = 1
                for x in range(1, 4):
                    if line[x+m] <58 and line[x+m] > 47:
                        word[x-1] = line[x+m]-48
                        #print(word[x-1])
                    else:
                        word[x-1] = 0
                        if check == 1:
                            w1 = x-1
                            #print(w1)
                            check = 0
                #print("\n")
                if w1-1 == 2:
                    x1 = word[0] * 100 + word[1] * 10 + word[2]
                elif w1-1 == 1:
                    x1 = word[0] * 10 + word[1]
                elif w1-1 == 0:
                    x1 = word[0]
            if j == 2:
                check = 1
                for x in range(1, 3):
                    #print("line")
                    #print(line[x+m])
                    if line[x+m] < 58 and line[x+m] > 47:
                        word2[x-1] = line[x+m]-48
                        #print("m")
                        #print(m)
                        #print(word2[x-1])
                    else:
                        word2[x-1] = 0
                        if check == 1:
                            w2 = x-1
                            #print("w2")
                            #print(w2)
                            check = 0
                #print("\n")
                if w2 - 1 == 1:
                    y1 = word2[0]*10+word2[1]
                    #print("y1")
                    #print(y1)
                elif w2 - 1 == 0:
                    y1 = word2[0]
                    #print("y1")
                    #print(y1)
                    
            if j == 3:
                check = 1
                for x in range(1, 4):
                    if line[x+m] <58 and line[x+m] > 47:
                        word3[x-1] = line[x+m]-48
                    else:
                        word3[x-1] = 0
                        if check == 1:
                            w3 = x-1
                            #print(w3)
                            check = 0
                if w3-1 == 2:
                    x2 = word3[0] * 100 + word3[1] * 10 + word3[2]
                    
                elif w3-1 == 1:
                    x2 = word3[0] * 10 + word3[1]
                elif w3-1 == 0:
                    x2 = word3[0]
                    
            if j == 4:
                check = 1
                for x in range(1, 3):
                    if line[x+m] < 58 and line[x+m] > 47:
                        word4[x-1] = line[x+m]-48
                    else:
                        word4[x-1] = 0
                        if check == 1:
                            w4 = x-1
                            #print(w4)
                            check = 0
                if w4 - 1 == 1:
                    y2 = word4[0]*10+word4[1]
                elif w4 - 1 == 0:
                    y2 = word4[0]
            k = k + 1
        
        '''if (x2-x1) != 0 and (y2-y1)/(x2-x1) > 1:
            s2.write("/turn/run -10 -0.1 \n".encode())
            s2.write("/goStraight/run 50\n".encode())
            print((y2-y1)/(x2-x1))
            print(str(x1)+","+str(y1)+","+str(x2)+","+str(y2))
        if (x2-x1) != 0 and (y2-y1)/(x2-x1) < -1:
            s2.write("/turn/run -10 0.1 \n".encode())
            s2.write("/goStraight/run 50\n".encode())
            print((y2-y1)/(x2-x1))
            print(str(x1)+","+str(y1)+","+str(x2)+","+str(y2))
        if (x2-x1) == 0:
            s2.write("/goStraight/run -100 \n".encode())'''
        print(str(x1)+","+str(y1)+","+str(x2)+","+str(y2))
        if x2 > x1 or x2 > 80:
            s2.write("/turn/run -10 -0.2 \n".encode())
            time.sleep(1)
            s2.write("/goStraight/run -10\n".encode())
            #print((y2-y1)/(x2-x1))
            #print(str(x1)+","+str(y1)+","+str(x2)+","+str(y2))
        elif x2 < x1 or x2 < 80:
            s2.write("/turn/run -10 0.2 \n".encode())
            time.sleep(1)
            s2.write("/goStraight/run -10\n".encode())
            #print((y2-y1)/(x2-x1))
            #print(str(x1)+","+str(y1)+","+str(x2)+","+str(y2))
        time.sleep(1)
        s2.write("/stop/run \n".encode())
                
            