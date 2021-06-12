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

        print_args = (d1, d2)
        s.write(("E,%d,%d\n"%print_args).encode())
        #time.sleep(5)
        '''if d1 <= d2:
            d22 = d2 - d1 + 11.5
            d11 = 3.5
            s.write("/goStraight/run 100 \n".encode())
            time.sleep(d22/1.875/7/3.14*2.5)
            print("straight", d22/1.875/7/3.14*2.5)
            s.write("/turn/run 100 -0.3 \n".encode())
            time.sleep(2.4)
            print("turn", 11*d1/2/9.065/3.14*2.5)
            s.write("/goStraight/run 100 \n".encode())
            time.sleep(d11/1.875/7/3.14*2.5)
            print("straight", d11/1.875/7/3.14*2.5)
            s.write("/stop/run \n".encode())
        else:
            d11 = d1 - d2 + 3.5
            d22 = 11.5
            s.write("/goStraight/run 100 \n".encode())
            time.sleep(d22/1.875/7/3.14*2.5)
            s.write("/turn/run 100 -0.3 \n".encode())
            time.sleep(2.4)
            s.write("/goStraight/run 100 \n".encode())
            time.sleep(d11/1.875/7/3.14*2.5)
            s.write("/stop/run \n".encode())'''
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
        print_args = (d1, d2)
        s.write(("W,%d,%d\n" % print_args).encode())
        #time.sleep(5)
        '''if d1 <= d2:
            d22 = d2 - d1 + 11.5
            d11 = 3.5
            s.write("/goStraight/run 100 \n".encode())
            time.sleep(d22/1.875/7/3.14*2.5)
            print("straight", d22/1.875/7/3.14*2.5)
            s.write("/turn/run 100 0.3 \n".encode())
            time.sleep(2.5)
            print("turn", 11*d1/2/9.065/3.14*2.5)
            s.write("/goStraight/run 100 \n".encode())
            time.sleep(d11/1.875/7/3.14*2.5)
            print("straight", d11/1.875/7/3.14*2.5)
            s.write("/stop/run \n".encode())
        else:
            d11 = d1 - d2 + 3.5
            d22 = 11.5
            s.write("/goStraight/run 100 \n".encode())
            time.sleep(d22/1.875/7/3.14*2.5)
            s.write("/turn/run 100 0.3 \n".encode())
            time.sleep(2.5)
            s.write("/goStraight/run 100 \n".encode())
            time.sleep(d11/1.875/7/3.14*2.5)
            s.write("/stop/run \n".encode())'''
    else:
        print ("not an arrow key!")
        return 1
    return 1

if len(sys.argv) < 1:
    print ("No port input")
s = serial.Serial(sys.argv[1])
while get():
    i = 0