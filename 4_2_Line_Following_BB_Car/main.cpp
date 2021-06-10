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

void openmv_thread(){
   uart.set_baud(9600);
   char buf[256];
   while(1){
      memset(buf, 0, 256);
      int i = 0;
      int end = 0;
      int mark;
      while (end == 0 && i < 256) {
         if(uart.readable()){
            char recv[1];
            uart.read(recv, sizeof(recv));
            pc.write(recv, sizeof(recv));
            //buf[i] = recv[0];
            
            //printf("%c", (buf[i]));
            
            //pc.write("%c", buf[i]);
            /*if (recv[0] == '\n') {
               end = 1;
               mark = i;
               break;
            }**/
         }
         i++;
      }
      /*for (int k = 0; k < mark; k++) {
         printf("%c", buf[i]);
      }
      printf("\n");*/
   }
}
int main(){
   
   thread.start(openmv_thread);
   char buf[256], outbuf[256];
   FILE *devin = fdopen(&xbee, "r");
   FILE *devout = fdopen(&xbee, "w");
   while (1) {
      memset(buf, 0, 256);
      for( int i = 0; ; i++ ) {
         char recv = fgetc(devin);
         if(recv == '\n') {
            printf("\n");
            break;
         }
         buf[i] = fputc(recv, devout);
      }
   RPC::call(buf, outbuf);
   }
}