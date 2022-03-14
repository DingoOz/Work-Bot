/*#include <NewPing.h>

//Bunch of pins
#define RLED 23
#define GLED 13
#define BLED 22

#define LIR A3
#define CIR A1
#define RIR A2

#define VDIV A0

//Motor pins
#define LF 12
#define LB 14
#define RF 9
#define RB 10

//Ultrasonic pins
#define USTRIGGER 6
#define USECHO 5

#define STOP_THRESH 2

//Other pin setup stuff
NewPing sonar(USTRIGGER, USECHO);
int LED[3] = {RLED, GLED, BLED};
int IR[3] = {LIR, CIR, RIR};
int MOT[2][2] = {{LF, LB}, {RF, RB}};

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

}

//The uncommented code should just make the robot rock back and forth every 8 seconds
void loop() {
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



#include <NewPing.h>

//Bunch of pins
#define RLED 23
#define GLED 13
#define BLED 22

#define LIR A3
#define CIR A1
#define RIR A2

#define VDIV A0

//Motor pins
#define LF 12
#define LB 14
#define RF 9
#define RB 10

//Ultrasonic pins
#define USTRIGGER 6
#define USECHO 5

#define STOP_THRESH 2

//Other pin setup stuff
NewPing sonar(USTRIGGER, USECHO);
int LED[3] = {RLED, GLED, BLED};
int IR[3] = {LIR, CIR, RIR};
int MOT[2][2] = {{LF, LB}, {RF, RB}};

int confirm; 
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
  *./


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

void followLineUntilJunction() {
  
  while (true) {
    unsigned long delta = millis() - t;
    if (millis() - t < 100) {
      delay(100-delta);
      continue;
    }
    t = millis();

    reading = 0;
    for (int i = 0; i < 3; i++) reading =+ checkIR(IR[i]) << i;

    Serial.println(reading);
    
    if (reading == 0b000 || reading == 0b101 || reading == 0b111) {
      if (confirm <= STOP_THRESH) {
        confirm++;
        continue;
      }
      else {
        setMotors(0, 0);
        digitalWrite(LED[0], 0); //red
        digitalWrite(LED[1], 0); //green
        digitalWrite(LED[2], 0); //blue
        return;
      }
    }
    confirm = 0;
    
    if (reading == 0b100) {
      setMotors(45, 80);
      digitalWrite(LED[0], 1); //red
      digitalWrite(LED[1], 0); //green
      digitalWrite(LED[2], 0); //blue
    }
    else if (reading == 0b001) {  
      setMotors(80, 45);
      digitalWrite(LED[0], 0); //red
      digitalWrite(LED[1], 0); //green
      digitalWrite(LED[2], 1); //blue
    }
    else if (reading == 0b110) {
      setMotors(50, 75);
      digitalWrite(LED[0], 1); //red
      digitalWrite(LED[1], 1); //green
      digitalWrite(LED[2], 0); //blue
    }
    else if (reading == 0b011) {  
      setMotors(75, 50);
      digitalWrite(LED[0], 0); //red
      digitalWrite(LED[1], 1); //green
      digitalWrite(LED[2], 1); //blue
    }
    else {
      setMotors(70, 70);
      digitalWrite(LED[0], 0); //red
      digitalWrite(LED[1], 1); //green
      digitalWrite(LED[2], 0); //blue
    }
  }
  
}
*/

#include <NewPing.h>

//Bunch of pins
#define RLED 23
#define GLED 13
#define BLED 22

#define LIR A3
#define CIR A1
#define RIR A2

#define VDIV A0

//Motor pins
#define LF 12
#define LB 14
#define RF 9
#define RB 10

//Ultrasonic pins
#define USTRIGGER 6
#define USECHO 5

#define STOP_THRESH 2

//Other pin setup stuff
NewPing sonar(USTRIGGER, USECHO);
int LED[3] = {RLED, GLED, BLED};
int IR[3] = {LIR, CIR, RIR};
int MOT[2][2] = {{LF, LB}, {RF, RB}};

int confirm;
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
  reading;
  t = millis();

}

//The uncommented code should just make the robot rock back and forth every 8 seconds
void loop() {


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


void followLineUntilJunction() {
  
  
  while (true) {
    unsigned long delta = millis() - t;
    if (millis() - t < 100) {
      delay(100-delta);
      continue;
    }
    t = millis();

    reading = 0;
    for (int i = 0; i < 3; i++) reading += checkIR(IR[i]) << i;

    Serial.println(reading);
    
    if (reading == 0b000 || reading == 0b101 || reading == 0b111) {
      if (confirm <= STOP_THRESH) {
        confirm++;
        continue;
      }
      else {
        setMotors(0, 0);
        digitalWrite(LED[0], 0); //red
        digitalWrite(LED[1], 0); //green
        digitalWrite(LED[2], 0); //blue
        
        return;
      }
    }
    confirm = 0;
    
    if (reading == 0b100) {
      setMotors(45, 80);
      digitalWrite(LED[0], 1); //red
      digitalWrite(LED[1], 0); //green
      digitalWrite(LED[2], 0); //blue
    }
    else if (reading == 0b001) {  
      setMotors(80, 45);
      digitalWrite(LED[0], 0); //red
      digitalWrite(LED[1], 0); //green
      digitalWrite(LED[2], 1); //blue
    }
    else if (reading == 0b110) {
      setMotors(50, 75);
      digitalWrite(LED[0], 1); //red
      digitalWrite(LED[1], 1); //green
      digitalWrite(LED[2], 0); //blue
    }
    else if (reading == 0b011) {  
      setMotors(75, 50);
      digitalWrite(LED[0], 0); //red
      digitalWrite(LED[1], 1); //green
      digitalWrite(LED[2], 1); //blue
    }
    else {
      setMotors(70, 70);
      digitalWrite(LED[0], 0); //red
      digitalWrite(LED[1], 1); //green
      digitalWrite(LED[2], 0); //blue
    }
  }
  
}
