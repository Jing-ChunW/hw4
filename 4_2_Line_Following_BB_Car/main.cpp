#include"mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"

BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(A1,A0); //tx,rx
Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BufferedSerial xbee(D1, D0);
Timer t;
BBCar car(pin5, pin6, servo_ticker);
Thread thread;
Ticker encoder_ticker;
volatile int count_new = 0;

void encoder_control() {
   count_new++;
}

int main(){
   
   encoder_ticker.attach(&encoder_control, 0.01);
   uart.set_baud(9600);
   char buf[256];
   while(1){
      memset(buf, 0, 256);
      int i = 0;
      int end = 0;
      int haveread = 0;
      int mark;
      int y = 0;
      char open[1];
      open[0] = 'L';
      uart.write(open, sizeof(open));
      count_new = 0;
      int nocount = 0;
      while (haveread == 0 && end == 0 && i < 256 && count_new < 100) {
         if(uart.readable()){
            char recv[1];
            uart.read(recv, sizeof(recv));
            y = 1;
            pc.write(recv, sizeof(recv));
            buf[i] = recv[0];
            if (recv[0] == '\n') {
               end = 1;
               mark = i;
               haveread = 1;
               break;
            }
            i++;
         }
         //printf("read\n");
         if (count_new == 99 && y == 0) {
            uart.write(open, sizeof(open));
            count_new = 0;
            printf("no\n");
            nocount++;
            if (nocount < 10) {
               car.goStraight(-5);
               ThisThread::sleep_for(50ms);
            }
         }
      }      /*for (int k = 0; k < mark; k++) {
         printf("%c", buf[k]);
      }
      printf("\n");*/
      //ThisThread::sleep_for(1000ms);
      int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
      int k = 0;
      for (k = 0; buf[k] != ','; k++) {
         x1 = 10*x1;
         if (buf[k] == 0) break;
         x1 += buf[k] - '0';
      }
      k++;
      for (; buf[k] != ','; k++) {
         y1 = 10*y1;
         if (buf[k] == 0) break;
         y1 += buf[k] - '0';
      }
      k++;
      for (; buf[k] != ','; k++) {
         x2 = 10*x2;
         if (buf[k] == 0) break;
         x2 += buf[k] - '0';
      }
      k++;
      for (; buf[k] != '\n'; k++) {
         y2 = 10*y2;
         if (buf[k] == 0) break;
         y2 += buf[k] - '0';
      }
      //printf("x1 = %d, y1 = %d, x2 = %d, y2 = %d\n", x1, y1, x2, y2);
      //ThisThread::sleep_for(1000ms);
      if (haveread == 1) {
         printf("x1 = %d, y1 = %d, x2 = %d, y2 = %d\n", x1, y1, x2, y2);
         if (((y1 - y2) <= 1 && y1 >= y2)|| ((y2 - y1) <= 1 && y2 >= y1)) {
            car.stop();
            ThisThread::sleep_for(2s);
            printf("jieur");
         } else {
            printf("hello\n");
            /*if (x1 < 80 and x2 < 80) {
               car.turn(-10, 0.2);
               ThisThread::sleep_for(1s);
            } else if (x1 > 80 and x2 > 80) {
               car.turn(-10, -0.2);
               ThisThread::sleep_for(1s);
            } else */if (x2 > x1) {
               if (x2 - x1 > 5) {
                  if (x2 - x1 > 130) {
                     car.stop();
                     ThisThread::sleep_for(2s);
                  } else {
                     car.turn(-20, -0.4);
                     ThisThread::sleep_for(2s);
                  }
              }
              else {
                  car.goStraight(-20);
                  ThisThread::sleep_for(2s);
              }
            } else {
               if (x1 - x2 > 5) {
                  if (x1 - x2 > 130) {
                     car.stop();
                     ThisThread::sleep_for(2s);
                  } else {
                     car.turn(-20, 0.4);
                     ThisThread::sleep_for(2s);
                  }
               } else {
                  car.goStraight(-20);
                  ThisThread::sleep_for(2s);
               }
            }
         }
         car.stop();
         ThisThread::sleep_for(1s);
      }
   }
}