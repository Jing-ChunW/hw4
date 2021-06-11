#include"mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
#include "mbed_rpc.h"
#include <math.h>
BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(A1,A0); //tx,rx
DigitalInOut ping(D11);
Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BufferedSerial xbee(D1, D0);
RpcDigitalOut myled1(LED1,"myled1");
RpcDigitalOut myled2(LED2,"myled2");
DigitalIn encoder0(D10);
DigitalIn encoder1(D12);

Timer t;
Thread thread;
Ticker encoder_ticker;
BBCar car(pin5, pin6, servo_ticker);
volatile int count_new = 0;
volatile int last0 = 0;
volatile int last1 = 0;
volatile int steps0 = 0;
volatile int steps1 = 0;

void ping_thread()
{
   float val;
   pc.set_baud(9600);
   while(1) {

      ping.output();
      ping = 0; wait_us(200);
      ping = 1; wait_us(5);
      ping = 0; wait_us(5);

      ping.input();
      while(ping.read() == 0);
      t.start();
      while(ping.read() == 1);
      val = t.read();
      printf("Ping=%lf\n", val*17700.4f);
      t.stop();
      t.reset();

      ThisThread::sleep_for(1s);
   }
}

void encoder_control() {
   int value0 = encoder0;
   if (!last0 && value0) steps0++;
   last0 = value0;
   int value1 = encoder1;
   if (!last1 && value1) steps1++;
   last1 = value1;
   count_new++;
}

int main(){
   
   thread.start(ping_thread);
   double pwm_table0[] = {-150, -120, -90, -60, -30, 0, 30, 60, 90, 120, 150};
   double speed_table0[] = {-16.741, -16.263, -15.147, -11.560, -5.182, 0.080, 6.378, 12.197, 15.227, 16.263, 16.742};
   double pwm_table1[] = {-150, -120, -90, -60, -30, 0, 30, 60, 90, 120, 150};
   double speed_table1[] = {-15.944, -15.546, -14.669, -11.958, -6.139, 0.000, 5.022, 11.002, 14.350, 15.546, 16.024};
   car.setCalibTable(11, pwm_table0, speed_table0, 11, pwm_table1, speed_table1);
   encoder_ticker.attach(&encoder_control, 0.01);
   uart.set_baud(9600);
   int double_check = 0;
   char buf[256];
   int enable = 1;

   while(1){
      memset(buf, 0, 256);
      int i = 0;
      int end = 0;
      int haveread = 0;
      char open[1];
      int y = 0;
      open[0] = 'A';
      uart.write(open, sizeof(open));
      count_new = 0;
      while (haveread == 0 && end == 0 && enable == 1 && i < 256 && count_new < 100) {   // read from openmv
         if(uart.readable()){
            char recv[1];
            uart.read(recv, sizeof(recv));
            //pc.write(recv, sizeof(recv));
            y = 1;
            buf[i] = recv[0];
            printf("%c\n", buf[i]);
            if (recv[0] == '\n') {
               end = 1;
               haveread = 1;
               break;
            }
         }
         i++;
      
         if (count_new == 99 && y == 0) {
            uart.write(open, sizeof(open));
            count_new = 0;
         }
      }
      
         double tx = 0, ty = 0, tz = 0;
         double txx = 0, tyy = 0, tzz = 0;
         double rx = 0, ry = 0, rz = 0;
         double rxx = 0, ryy = 0, rzz = 0;
         double goal_distance = 0, goal_angle = 0;
         int w = 0;
         int minus = 0;
         for (w = 0; buf[w] != '.'; w++) {
            tx = 10*tx;
            if (buf[w] == 0) break;
            if (buf[w] == '-') minus = 1;
            else tx += buf[w] - '0';
         }
         w++;
         for (; buf[w] != ','; w++) {
            txx = 10*txx;
            if (buf[w] == 0) break;
            txx += buf[w] - '0';
         }
         tx += txx/1000000;
         if (minus == 1) tx = -tx;
         w++;
         minus = 0;
         for (; buf[w] != '.'; w++) {
            ty = 10*ty;
            if (buf[w] == 0) break;
            if (buf[w] == '-') minus = 1;
            else ty += buf[w] - '0';
         }
         w++;
         for (; buf[w] != ','; w++) {
            tyy = 10*tyy;
            if (buf[w] == 0) break;
            tyy += buf[w] - '0';
         }
         ty += tyy/1000000;
         if (minus == 1) ty = -ty;
         w++;
         minus = 0;
         for (; buf[w] != '.'; w++) {
            tz = 10*tz;
            if (buf[w] == 0) break;
            if (buf[w] == '-') minus = 1;
            else tz += buf[w] - '0';
         }
         w++;
         for (; buf[w] != ','; w++) {
            tzz = 10*tzz;
            if (buf[w] == 0) break;
            tzz += buf[w] - '0';
         }
         tz += tzz/1000000;
         if (minus == 1) tz = -tz;
         w++;
         for (; buf[w] != '.'; w++) {
            rx = 10*rx;
            if (buf[w] == 0) break;
            rx += buf[w] - '0';
         }
         w++;
         for (; buf[w] != ','; w++) {
            rxx = 10*rxx;
            if (buf[w] == 0) break;
            rxx += buf[w] - '0';
         }
         rx += rxx/1000000;
         w++;
         for (; buf[w] != '.'; w++) {
            ry = 10*ry;
            if (buf[w] == 0) break;
            ry += buf[w] - '0';
         }
         w++;
         for (; buf[w] != ','; w++) {
            ryy = 10*ryy;
            if (buf[w] == 0) break;
            ryy += buf[w] - '0';
         }
         ry += ryy/1000000;
         w++;
         for (; buf[w] != '.'; w++) {
            rz = 10*rz;
            if (buf[w] == 0) break;
            rz += buf[w] - '0';
         }
         w++;
         for (; buf[w] != '\n'; w++) {
            rzz = 10*rzz;
            if (buf[w] == 0) break;
            rzz += buf[w] - '0';
         }
         rz += rzz/1000000;
         //printf("%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n", tx, ty, tz, rx, ry, rz);
         if (haveread == 1) {
            printf("%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n", tx, ty, tz, rx, ry, rz);
            if (double_check >= 1) {
               goal_distance = sqrt(pow(tx, 2) + pow(ty, 2) + pow(tz, 2))*2.8;
               goal_angle = ry;
               int angle_diff = 0;
               if (goal_angle > 0 && goal_angle < 180) {
                  car.turn(-20, 0.1);
                  angle_diff = (goal_angle - 0);
                  while (steps0*6.5*3.14/32 < 11*2*3.14*(90 - angle_diff)/360) {
                     ThisThread::sleep_for(100ms);
                  }
                  car.goStraight(-20);
                  while (steps0*6.5*3.14/32 < goal_distance*sin(angle_diff)-5) {
                     ThisThread::sleep_for(100ms);
                  }
                  car.turn(-20, -0.1);
                  while (steps1*6.5*3.14/32 < 11*2*3.14*110/360) {
                     ThisThread::sleep_for(100ms);
                  }
               } else if (goal_angle > 180 && goal_angle < 360) {
                  car.turn(-20, -0.05);
                  angle_diff = 360 - goal_angle;
                  while (steps1*6.5*3.14/32 < 11*2*3.14*(90 - angle_diff)/360) {
                     ThisThread::sleep_for(100ms);
                  }
                  car.goStraight(-20);
                  while (steps0*6.5*3.14/32 < goal_distance*sin(angle_diff)-10) {
                     ThisThread::sleep_for(100ms);
                  }
                  car.turn(-20, 0.05);
                  while (steps0*6.5*3.14/32 < 11*2*3.14*110/360) {
                     ThisThread::sleep_for(100ms);
                  }
               }
               car.stop();
               enable = 0;
            } else {
               double_check++;
            }
            /*car.goStraight(-20);
            while (steps0*6.5*3.14/32 < goal_distance) {
               ThisThread::sleep_for(100ms);
            }
            car.stop();*/
            
         }   

   }


}