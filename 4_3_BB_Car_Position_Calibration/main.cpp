#include"mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
#include "mbed_rpc.h"
BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(A1,A0); //tx,rx
DigitalInOut ping(D11);
Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BufferedSerial xbee(D1, D0);
RpcDigitalOut myled1(LED1,"myled1");
RpcDigitalOut myled2(LED2,"myled2");
Timer t;
Thread thread;
Thread thread2;
BBCar car(pin5, pin6, servo_ticker);
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

void openmv_thread()
{
   uart.set_baud(9600);
   /*char buf[256];
   int i = 0, j = 0, num1 = 0, mark;
   float Rx = 0.0;
   for (j = 0; j < 256; j++) buf[j] = 0;*/
   while(1){
      if(uart.readable()){
            char recv[1];
            uart.read(recv, sizeof(recv));
            pc.write(recv, sizeof(recv));
            /*buf[i] = recv[1];
            i++;
            if (recv[1] == "\n") {
                if (buf[0] == "T") {
                    for (j = 0; j < i-1;j++) {
                        if (buf[j] == ":") {
                            num1 = num1 + 1;
                            mark = j;
                        }
                        if (num1 == 4) {
                            if (j-mark == 1) Rx = (buf[j]-48)*100;
                            if (j-mark == 2) Rx = (buf[j]-48)*10+Rx;
                            if (j-mark == 3) Rx = (buf[j]-48) + Rx;
                            if (j-(mark+4) == 1) Rx = Rx + (buf[j]-48)*0.1;
                            if (j-(mark+4) == 2) Rx = Rx + (buf[j]-48)*0.01;
                            if (j-(mark+4) == 3) Rx = Rx + (buf[j]-48)*0.001;
                            if (j-(mark+4) == 4) Rx = Rx + (buf[j]-48)*0.0001;
                            if (j-(mark+4) == 5) Rx = Rx + (buf[j]-48)*0.00001;
                            if (j-(mark+4) == 6) Rx = Rx + (buf[j]-48)*0.000001;

                        }
                    }
                }
                i = 0;
                num1 = 0;
                Rx = 0.0;
            }
            printf("%f\n", Rx);*/
      }
   }
}

int main(){
   
   thread.start(ping_thread);
   thread2.start(openmv_thread);
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