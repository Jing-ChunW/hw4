#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BufferedSerial xbee(D1, D0);

BBCar car(pin5, pin6, servo_ticker);
DigitalIn encoder0(D10);
DigitalIn encoder1(D12);
Ticker encoder_ticker;
volatile int count_new = 0;
volatile int last0 = 0;
volatile int last1 = 0;
volatile int steps0 = 0;
volatile int steps1 = 0;
void encoder_control() {
   int value0 = encoder0;
   if (!last0 && value0) steps0++;
   last0 = value0;
   int value1 = encoder1;
   if (!last1 && value1) steps1++;
   last1 = value1;
   count_new++;
}
int main() {
   double pwm_table0[] = {-150, -120, -90, -60, -30, 0, 30, 60, 90, 120, 150};
   double speed_table0[] = {-16.741, -16.263, -15.147, -11.560, -5.182, 0.080, 6.378, 12.197, 15.227, 16.263, 16.742};
   double pwm_table1[] = {-150, -120, -90, -60, -30, 0, 30, 60, 90, 120, 150};
   double speed_table1[] = {-15.944, -15.546, -14.669, -11.958, -6.139, 0.000, 5.022, 11.002, 14.350, 15.546, 16.024};
   car.setCalibTable(11, pwm_table0, speed_table0, 11, pwm_table1, speed_table1);
   encoder_ticker.attach(&encoder_control, 0.01);
   char buf[256], outbuf[256];
   FILE *devin = fdopen(&xbee, "r");
   FILE *devout = fdopen(&xbee, "w");
   while (1) {
      memset(buf, 0, 256);
      int haveread = 0;
      int end = 0;
      for( int i = 0; ; i++ ) {
         char recv = fgetc(devin);
         if(recv == '\n') {
            //printf("\r\n");
            haveread = 1;
            end = i;
            break;
         }
         buf[i] = fputc(recv, devout);
      }
      printf("%s", buf);
      buf[end] = '\n';
      int w = 0;
      int d1 = 0, d2 = 0;
      if (haveread == 1) {
         if (buf[0] == 'E') {
            for (w = 2; buf[w] != ','; w++) {
               d1 = 10*d1;
               if (buf[w] == 0) break;
               d1 += buf[w] - '0';
            }
            w++;
            for (;buf[w] != '\n'; w++) {
               d2 = 10*d2;
               if (buf[w] == 0) break;
               d2 += buf[w] - '0';
            }
            printf("%d, %d\n", d1, d2);
            car.goStraight(20);
            steps0 = 0;
            while(steps0*6.5*3.14/32 < d2+11.5) {
               ThisThread::sleep_for(100ms);
            }
            steps0 = 0;
            steps1 = 0;
            car.turn(20, -0.1);
            while (steps1*6.5*3.14/32 < 11*2*3.14/4) {
               ThisThread:: sleep_for(100ms);
            }
            steps1 = 0;
            steps0 = 0;
            car.goStraight(20);
            while(steps0*6.5*3.14/32 < d1+3.5) {
               ThisThread::sleep_for(100ms);
            }
         } else if (buf[0] == 'W') {
            for (w = 2; buf[w] != ','; w++) {
               d1 = 10*d1;
               if (buf[w] == 0) break;
               d1 += buf[w] - '0';
            }
            w++;
            for (;buf[w] != '\n'; w++) {
               d2 = 10*d2;
               if (buf[w] == 0) break;
               d2 += buf[w] - '0';
            }
            printf("%d, %d\n", d1, d2);
            steps0 = 0;
            car.goStraight(20);
            while(steps0*6.5*3.14/32 < d2+11.5) {
               ThisThread::sleep_for(100ms);
            }
            steps0 = 0;
            car.turn(20, 0.1);
            while (steps0*6.5*3.14/32 < 11*2*3.14/4) {
               ThisThread:: sleep_for(100ms);
            }
            steps0 = 0;
            car.goStraight(20);
            while(steps0*6.5*3.14/32 < d1+3.5) {
               ThisThread::sleep_for(100ms);
            }
         }
         car.stop();
      }
   //RPC::call(buf, outbuf);
   }
}