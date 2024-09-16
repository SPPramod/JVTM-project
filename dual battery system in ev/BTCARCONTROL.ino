///*
//  Blink
//
//  Turns an LED on for one second, then off for one second, repeatedly.
//
//  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
//  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
//  the correct LED pin independent of which board is used.
//  If you want to know what pin the on-board LED is connected to on your Arduino
//  model, check the Technical Specs of your board at:
//  https://www.arduino.cc/en/Main/Products
//
//  modified 8 May 2014
//  by Scott Fitzgerald
//  modified 2 Sep 2016
//  by Arturo Guadalupi
//  modified 8 Sep 2016
//  by Colby Newman
//
//  This example code is in the public domain.
//
//  http://www.arduino.cc/en/Tutorial/Blink
//*/
//
//// the setup function runs once when you press reset or power the board
//void setup() {
//  // initialize digital pin LED_BUILTIN as an output.
//  pinMode(LED_BUILTIN, OUTPUT);
//}
//
//// the loop function runs over and over again forever
//void loop() {
//  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
//  delay(1000);                       // wait for a second
//  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
//  delay(1000);                       // wait for a second
//}

///////////////////////////////////////////// 
//        RemoteXY include library         // 
///////////////////////////////////////////// 

/* RemoteXY select connection mode and include library */ 
#define REMOTEXY_MODE__SOFTWARESERIAL 
#include <SoftwareSerial.h> 
#include <RemoteXY.h> 

/* RemoteXY connection settings */
#define REMOTEXY_SERIAL_RX 2 
#define REMOTEXY_SERIAL_TX 3 
#define REMOTEXY_SERIAL_SPEED 9600 

/* RemoteXY configurate  */  
unsigned char RemoteXY_CONF[] = 
  { 255,3,0,0,0,29,0,16,13,0,5,15,49,9,43,43,1,26,31,2,
  0,6,7,32,15,5,26,31,31,79,78,0,79,70,70,0 }; 
   
/* this structure defines all the variables of your control interface */  
struct { 

    /* input variable */
  signed char joystick_1_x; /* =-100..100 x-coordinate joystick position */
  signed char joystick_1_y; /* =-100..100 y-coordinate joystick position */
  unsigned char switch_1; /* =1 if switch ON and =0 if OFF */

    /* other variable */
  unsigned char connect_flag;  /* =1 if wire connected, else =0 */

} RemoteXY; 

///////////////////////////////////////////// 
//           END RemoteXY include          // 
///////////////////////////////////////////// 
/* defined the right motor control pins */
#define PIN_MOTOR_RIGHT_UP 9
#define PIN_MOTOR_RIGHT_DN 10
#define PIN_MOTOR_RIGHT_SPEED 7

/* defined the left motor control pins */
#define PIN_MOTOR_LEFT_UP 11
#define PIN_MOTOR_LEFT_DN 12
#define PIN_MOTOR_LEFT_SPEED 8

/* defined the LED pin */
#define PIN_LED 13


/* defined two arrays with a list of pins for each motor */
unsigned char RightMotor[3] = 
  {PIN_MOTOR_RIGHT_UP, PIN_MOTOR_RIGHT_DN, PIN_MOTOR_RIGHT_SPEED};
unsigned char LeftMotor[3] = 
  {PIN_MOTOR_LEFT_UP, PIN_MOTOR_LEFT_DN, PIN_MOTOR_LEFT_SPEED};

/*
   speed control of the motor
   motor - pointer to an array of pins
   v - motor speed can be set from -100 to 100
*/
void Wheel (unsigned char * motor, int v)
{
  if (v>100) v=100;
  if (v<-100) v=-100;
  if (v>0) {
    digitalWrite(motor[0], HIGH);
    digitalWrite(motor[1], LOW);
    analogWrite(motor[2], v*2.55);
  }
  else if (v<0) {
    digitalWrite(motor[0], LOW);
    digitalWrite(motor[1], HIGH);
    analogWrite(motor[2], (-v)*2.55);
  }
  else {
    digitalWrite(motor[0], LOW);
    digitalWrite(motor[1], LOW);
    analogWrite(motor[2], 0);
  }
}

void setup()
{
  /* initialization pins */
  pinMode (PIN_MOTOR_RIGHT_UP, OUTPUT);
  pinMode (PIN_MOTOR_RIGHT_DN, OUTPUT);
  pinMode (PIN_MOTOR_LEFT_UP, OUTPUT);
  pinMode (PIN_MOTOR_LEFT_DN, OUTPUT);
  pinMode (PIN_LED, OUTPUT);

  /* initialization module RemoteXY */
  RemoteXY_Init ();

}

void loop()
{
  /* event handler module RemoteXY */
  RemoteXY_Handler ();

  /* manage LED pin */
  digitalWrite (PIN_LED, (RemoteXY.switch_1==0)?LOW:HIGH);

  /* manage the right motor */
  Wheel (RightMotor, RemoteXY.joystick_1_y - RemoteXY.joystick_1_x);
  /* manage the left motor */
  Wheel (LeftMotor, RemoteXY.joystick_1_y + RemoteXY.joystick_1_x);
}
