/*
 * QUT Robotics Work Bot
 * Hardware is based on a Teensy 2.0, SR04, 3 IR sensors and 
 * custom H-bridges driving two 6v geared DC motors.
 */


#include <NewPing.h>  //to drive the Ultrasonic SR04 sensor

//
***** Pin Defintions *****

// RGB LED 
#define RLED 23
#define GLED 13
#define BLED 22

#define LIR A3
#define CIR A1
#define RIR A2

// Battery Voltage ADC
#define VDIV A0

//Motor H-Bridge
#define LF 12
#define LB 14
#define RF 9
#define RB 10

//Ultrasonic Sensor SR04
#define USTRIGGER 6
#define USECHO 5
//*************************


#define STOP_THRESH 2
#define STOP_THRESH_DIST 5

//Other pin setup stuff
NewPing sonar(USTRIGGER, USECHO);
int LED[3] = {RLED, GLED, BLED};
int IR[3] = {LIR, CIR, RIR};
int MOT[2][2] = {{LF, LB}, {RF, RB}};

int confirm; 
int confirm_dist; 
uint8_t reading;
unsigned long t;;

//Sets up everything
void setup() {
  //LEDS and IR
  for (int i = 0; i < 3; i++) {
    pinMode(LED[i], OUTPUT);
    pinMode(IR[i], INPUT);
  }

  //Voltage Divider, for checking battery
  pinMode(VDIV, INPUT);

  //Serial, for outputting... stuff.
  Serial.begin(9600);

  confirm = 0; 
  confirm_dist = 0;
  reading;
  t = millis();

}

//The uncommented code should just make the robot rock back and forth every 8 seconds
void loop() {
  /*
  for (int i = 0; i < 200; i++) {
    setMotors(i, i);
    delay(10);
  }
  for (int i = 200; i > 0; i--) {
    setMotors(i, i);
    delay(10);
  }
  for (int i = 0; i < 200; i++) {
    setMotors(-i, -i);
    delay(10);
  }
  for (int i = 200; i > 0; i--) {
    setMotors(-i, -i);
    delay(10);
  }

  delay(1000);
  
  
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED[(i-1>=0) ? i-1 : 2], 0);
    digitalWrite(LED[i], 1);
    //Serial.println(sonar.ping_cm());
    //Serial.println(checkIR(IR[i]));
    delay(1000);
  }
  */

  //Serial.println(sonar.ping_cm());


  if (sonar.ping_cm() < 3) {
    if (confirm_dist >= STOP_THRESH_DIST) {
      //wallahead();
      confirm_dist = 0;
      Serial.println("hit");
    }
    else{
      confirm_dist++;
    }      
  }

  followLineUntilJunction();
  
  
}

//returns true if on white, false if on black/edge
bool checkIR(int ir) {
  return analogRead(ir) < 500;
}

//returns true if the battery has enough juice
bool checkBatt() {
  return analogRead(VDIV) > 512;
}


//each motor between -255 and 255 (though best values are < 200)
void setMotors(int left, int right) {
  int val[2] = {left, right};
  for (int i = 0; i < 2; i++) {
    if (val[i] == 0) for (int j = 0; j < 2; j++) analogWrite(MOT[i][j], 0);
    else {
      bool dir = val[i] > 0;
      analogWrite(MOT[i][!dir], 0);
      analogWrite(MOT[i][dir], abs(val[i]));
    }
  }
}

void wallahead() {
  setMotors(-50, 50);
  delay(1500);
}

void followLineUntilJunction() {
  
  
  //while (true) {
    unsigned long delta = millis() - t;
    if (millis() - t < 100) {
      delay(100-delta);
      //continue;
    }
    t = millis();

    reading = 0;
    for (int i = 0; i < 3; i++) reading += checkIR(IR[i]) << i;

    Serial.println(reading);
    
    if (reading == 0b000 || reading == 0b101 || reading == 0b111) {
      if (confirm <= STOP_THRESH) {
        confirm++;
        //continue;
      }
      else {
        //setMotors(0, 0);
        digitalWrite(LED[0], 0); //red
        digitalWrite(LED[1], 0); //green
        digitalWrite(LED[2], 0); //blue
        confirm = 0;
        return;
      }
    }
    
    
    if (reading == 0b011) {
      //setMotors(45, 80);
      setMotors(80, 30);
      digitalWrite(LED[0], 1); //red
      digitalWrite(LED[1], 0); //green
      digitalWrite(LED[2], 0); //blue
    }
    else if (reading == 0b110) {  
      //setMotors(80, 45);
      setMotors(30, 80);
      digitalWrite(LED[0], 0); //red
      digitalWrite(LED[1], 0); //green
      digitalWrite(LED[2], 1); //blue
    }
    else if (reading == 0b001) {
      //setMotors(50, 75);
      setMotors(100, 30);
      digitalWrite(LED[0], 1); //red
      digitalWrite(LED[1], 1); //green
      digitalWrite(LED[2], 0); //blue
    }
    else if (reading == 0b100) {  
      //setMotors(75, 30);
      setMotors(30, 100);
      digitalWrite(LED[0], 0); //red
      digitalWrite(LED[1], 1); //green
      digitalWrite(LED[2], 1); //blue
    }
    else if (reading == 0b101) {
      setMotors(50, 50);
      digitalWrite(LED[0], 0); //red
      digitalWrite(LED[1], 1); //green
      digitalWrite(LED[2], 0); //blue
    }
  //}
  
}
