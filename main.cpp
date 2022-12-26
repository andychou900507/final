#include "mbed.h"
#include "bbcar.h"
  #include "PwmIn.h"
#include <cstdio>
  #include <iostream>
  #include "uLCD_4DGL.h"

  #define CENTER_BASE 1500
  #define unitsFC 360                          // Units in a full circle
  #define dutyScale 1000                       // Scale duty cycle to 1/000ths
  #define dcMin 29                             // Minimum duty cycle
  #define dcMax 971                            // Maximum duty cycle
  #define q2min unitsFC/4                      // For checking if in 1st uadrant
  #define q3max q2min * 3                      // For checking if in 4th uadrant

Ticker servo_ticker;
Ticker servo_feedback_ticker;

PwmOut pin11(D11), pin13(D13);
PwmIn pin10(D10), pin12(D12);
BBCar car(pin11, pin10, pin13, pin12, servo_ticker, servo_feedback_ticker);
BusInOut qti_pin(D4,D5,D6,D7);
uLCD_4DGL uLCD(D1, D0, D2);

BufferedSerial pc(USBTX, USBRX);
DigitalInOut ping(D9);
Timer t;

  volatile int angle1, targetAngle1 = 3;              // Global shared ariables
  volatile int Kp1 = 1;                          // Proportional constant
  volatile float tCycle1;
  volatile int theta1;
  volatile int thetaP1;
  volatile int turns1 = 0;
  volatile int angle2, targetAngle2 = 3;              // Global shared ariables
  volatile int Kp2 = 1;                          // Proportional constant
  volatile float tCycle2;
  volatile int theta2;
  volatile int thetaP2;
  volatile int turns2 = 0;

  void feedback360(){                           // Position monitoring
     tCycle1 = pin10.period();
     int dc1 = dutyScale * pin10.dutycycle();
     theta1 = (unitsFC - 1) -                   // Calculate angle
              ((dc1 - dcMin) * unitsFC)
              / (dcMax - dcMin + 1);
     if(theta1 < 0)                             // Keep theta valid
        theta1 = 0;
     else if(theta1 > (unitsFC - 1))
        theta1 = unitsFC - 1;

     // If transition from quadrant 4 to
     // quadrant 1, increase turns count.
     if((theta1 < q2min) && (thetaP1 > q3max))
        turns1++;
     // If transition from quadrant 1 to
     // quadrant 4, decrease turns count.
     else if((thetaP1 < q2min) && (theta1 > q3max))
        turns1 --;

     // Construct the angle measurement from the turns count and
     // current theta value.
     if(turns1 >= 0)
        angle1 = (turns1 * unitsFC) + theta1;
     else if(turns1 <  0)
        angle1 = ((turns1 + 1) * unitsFC) - (unitsFC - theta1);

     thetaP1 = theta1;                           // Theta previous for next rep


     tCycle2 = pin12.period();
     int dc2 = dutyScale * pin12.dutycycle();
     theta2 = (unitsFC - 1) -                   // Calculate angle
              ((dc2 - dcMin) * unitsFC)
              / (dcMax - dcMin + 1);
     if(theta2 < 0)                             // Keep theta valid
        theta2 = 0;
     else if(theta2 > (unitsFC - 1))
        theta2 = unitsFC - 1;

     // If transition from quadrant 4 to
     // quadrant 1, increase turns count.
     if((theta2 < q2min) && (thetaP2 > q3max))
        turns2++;
     // If transition from quadrant 1 to
     // quadrant 4, decrease turns count.
     else if((thetaP2 < q2min) && (theta2 > q3max))
        turns2 --;

     // Construct the angle measurement from the turns count and
     // current theta value.
     if(turns2 >= 0)
        angle2 = (turns2 * unitsFC) + theta2;
     else if(turns2 <  0)
        angle2 = ((turns2 + 1) * unitsFC) - (unitsFC - theta2);

     thetaP2 = theta2;                           // Theta previous for next rep
  }


int main() {

   parallax_qti qti1(qti_pin);
   int pattern;
   int count_2 = 0;
   int count3 = 0;
   int count_m = 0;
   int count_m_2 = 0;
   servo_feedback_ticker.attach(&feedback360, 5ms);

   float val;
   int distance_uLCD;
   pc.set_baud(9600);



   car.goStraight(-30);
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

      pattern = (int)qti1;
      distance_uLCD = val*17150.0f;
     // printf("%d\n",pattern);
      if (pattern == 0b1000) {
          if (distance_uLCD < 10)  {
              car.stop();
              uLCD.text_width(1); // 4X size text
              uLCD.text_height(1);
              uLCD.color(RED);
              uLCD.locate(1, 2);
              uLCD.printf("Watch out!\n");
          }
          else {
            uLCD.text_width(1); // 4X size text
            uLCD.text_height(1);
            uLCD.color(RED);
            uLCD.locate(1, 2);
            uLCD.printf("Go ahead !\n");
            car.turn(-40, 0.3);
            count_2 = 0;
            count3 = 0;
          }
      }
      else if (pattern == 0b1100) {
          if (distance_uLCD < 10)  {
            car.stop();
            uLCD.text_width(1); // 4X size text
            uLCD.text_height(1);
            uLCD.color(RED);
            uLCD.locate(1, 2);
            uLCD.printf("Watch out!\n");
          }
          else {
            car.turn(-40, 0.3);
            uLCD.text_width(1); // 4X size text
            uLCD.text_height(1);
            uLCD.color(RED);
            uLCD.locate(1, 2);
            uLCD.printf("Go ahead !\n");
            count_2 = 0;
            count3 = 0;
          }
      }
      else if (pattern == 0b0100) {
          if (distance_uLCD < 10)  {
            car.stop();
            uLCD.text_width(1); // 4X size text
            uLCD.text_height(1);
            uLCD.color(RED);
            uLCD.locate(1, 2);
            uLCD.printf("Watch out!\n");
          }
          else {
            uLCD.text_width(1); // 4X size text
            uLCD.text_height(1);
            uLCD.color(RED);
            uLCD.locate(1, 2);
            uLCD.printf("Go ahead !\n");
            car.turn(-40, 0.3);
            count_2 = 0;
            count3 = 0;
          }
      }
      else if (pattern == 0b0110) {
          if (distance_uLCD < 10)  {
            car.stop();
            uLCD.text_width(1); // 4X size text
            uLCD.text_height(1);
            uLCD.color(RED);
            uLCD.locate(1, 2);
            uLCD.printf("Watch out!\n");
          }
          else {
            uLCD.text_width(1); // 4X size text
            uLCD.text_height(1);
            uLCD.color(RED);
            uLCD.locate(1, 2);
            uLCD.printf("Go ahead !\n");
            car.goStraight(-30);
            count_2 = 0;
            count3 = 0;
          }
      }
      else if (pattern == 0b0010) {
          if (distance_uLCD < 10)  {
            car.stop();
            uLCD.text_width(1); // 4X size text
            uLCD.text_height(1);
            uLCD.color(RED);
            uLCD.locate(1, 2);
            uLCD.printf("Watch out!\n");
          }
          else {
            uLCD.text_width(1); // 4X size text
            uLCD.text_height(1);
            uLCD.color(RED);
            uLCD.locate(1, 2);
            uLCD.printf("Go ahead !\n");
            car.turn(-40, -0.3);
            count_2 = 0;
            count3 = 0;
          }
      }
      else if (pattern == 0b0011) {
          if (distance_uLCD < 10)  {
            car.stop();
            uLCD.text_width(1); // 4X size text
            uLCD.text_height(1);
            uLCD.color(RED);
            uLCD.locate(1, 2);
            uLCD.printf("Watch out!\n");
          }
          else {
            uLCD.text_width(1); // 4X size text
            uLCD.text_height(1);
            uLCD.color(RED);
            uLCD.locate(1, 2);
            uLCD.printf("Go ahead !\n");
            car.turn(-40, -0.3);
            count_2 = 0;
            count3 = 0;
          }
      }
      else if (pattern == 0b0001) {
          if (distance_uLCD < 10)  {
            car.stop();
            uLCD.text_width(1); // 4X size text
            uLCD.text_height(1);
            uLCD.color(RED);
            uLCD.locate(1, 2);
            uLCD.printf("Watch out!\n");
          }
          else {
            uLCD.text_width(1); // 4X size text
            uLCD.text_height(1);
            uLCD.color(RED);
            uLCD.locate(1, 2);
            uLCD.printf("Go ahead !\n");
            car.turn(-40, -0.3);
            count_2 = 0;
            count3 = 0;
          }
      }
      else if (pattern == 0b1111) {
          if (count_2 < 200) {
            count_2++;
            car.stop(); 
            if (count_2 > 180) {
                uLCD.text_width(1); // 4X size text
                uLCD.text_height(1);
                uLCD.color(RED);
                uLCD.locate(1, 2);
                uLCD.printf("distance = %3d\n", distance_uLCD);
            }
          } else {
            if (distance_uLCD < 20) {
                car.turn(-50, 0.5);
                ThisThread::sleep_for(1000ms);
            }
            else car.goStraight(-30);
            count3 = 0;
          }
      }
      else if (pattern == 0b0111) {
          if (count_m < 80) {
              count_m++;
              car.goStraight(-30);
          } else {
              count_m = 0;
              car.turn(-70, -0.1);
              ThisThread::sleep_for(1000ms);
          }
          count_2 = 0;
          count3 = 0;
      }
      else if (pattern == 0b1110) {
          if (count_m_2 < 80) {
              count_m_2++;
              car.goStraight(-30);
          } else {
              count_m_2 = 0;
              car.turn(-70, 0.1);
              ThisThread::sleep_for(1000ms);
          }
          count_2 = 0;
          count3 = 0;
      }
      else if (pattern == 0b0000) {
          if (count3 < 100) {
              count3++;
              car.goStraight(-30);
          }
          else {
              car.goStraight(60);
              ThisThread::sleep_for(1000ms);
              count3 = 0;
          }
          
          count_2 = 0;
      }
      else {
          if (distance_uLCD < 10)  {
            car.stop();
            uLCD.text_width(1); // 4X size text
            uLCD.text_height(1);
            uLCD.color(RED);
            uLCD.locate(1, 2);
            uLCD.printf("Watch out!\n");
          }
          else {
            uLCD.text_width(1); // 4X size text
            uLCD.text_height(1);
            uLCD.color(RED);
            uLCD.locate(1, 2);
            uLCD.printf("Go ahead !\n");
            car.goStraight(-30);
            count_2 = 0;
            count3 = 0;
          }
      }
      printf("pattern %d", pattern);
      t.stop();
      t.reset();
      ThisThread::sleep_for(10ms);
   }
}