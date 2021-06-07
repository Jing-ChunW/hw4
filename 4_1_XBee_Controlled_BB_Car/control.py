import time
import serial
import sys,tty,termios
class _Getch:
    def __call__(self):
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        try:
            tty.setraw(sys.stdin.fileno())
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch

def get():
    inkey = _Getch()
    k2 = inkey()
    t = 7
    if k2=='q':
        print ("quit")
        return 0
    elif k2=='E':
        inkey()
        inkey()
        inkey()
        inkey()
        d1 = int(inkey())*10
        d1 = int(inkey()) + d1
        inkey()
        d2 = int(inkey())*10
        d2 = int(inkey()) + d2
        print ("East", d1, d2)
        if d1 < 8:
            s.write("/turn/run 100 0.3 \n".encode())
            time.sleep(2.5)
            s.write("/turn/run 100 -0.3 \n".encode())
            time.sleep(2.2)
            s.write("/goStraight/run -100 \n".encode())
            time.sleep(16*t/100)
            d1 = 16 + d1
        if d2 < 8:
            s.write("/goStraight/run -100 \n".encode())
            time.sleep(16*t/100)
            d2 = 16 + d2
        d22 = d2 - 9+11.5
        d11 = d1 - 9+3.5
        s.write("/goStraight/run 100 \n".encode())
        time.sleep(d22*t/100)
        s.write("/turn/run 100 -0.3 \n".encode())
        time.sleep(2.5)
        s.write("/goStraight/run 100 \n".encode())
        time.sleep(d11*t/100)
        s.write("/stop/run \n".encode())
    elif k2=='W':
        inkey()
        inkey()
        inkey()
        inkey()
        d1 = int(inkey())*10
        d1 = int(inkey()) + d1
        inkey()
        d2 = int(inkey())*10
        d2 = int(inkey()) + d2
        print ("West", d1, d2)
        if d1 < 8:
            s.write("/turn/run 100 -0.3 \n".encode())
            time.sleep(2.8)
            s.write("/turn/run 100 0.3 \n".encode())
            time.sleep(2.4)
            s.write("/goStraight/run -100 \n".encode())
            time.sleep(16*t/100)
            d1 = 16 + d1
        if d2 < 8:
            s.write("/goStraight/run -100 \n".encode())
            time.sleep(16*t/100)
            d2 = 16 + d2
        d22 = d2 - 9+ 11.5
        d11 = d1 - 9+3.5
        s.write("/goStraight/run 100 \n".encode())
        time.sleep(d22*t/100)
        s.write("/turn/run 100 0.3 \n".encode())
        time.sleep(2.3)
        s.write("/goStraight/run 100 \n".encode())
        time.sleep(d11*t/100)
        s.write("/stop/run \n".encode())
    else:
        print ("not an arrow key!")
        return 1
    return 1

if len(sys.argv) < 1:
    print ("No port input")
s = serial.Serial(sys.argv[1])
while get():
    i = 0