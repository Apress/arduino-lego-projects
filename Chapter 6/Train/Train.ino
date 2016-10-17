#include <legopowerfunctions.h>

int fwdSpeed[] = {PWM_FLT, PWM_FWD1, PWM_FWD2, 
    PWM_FWD3, PWM_FWD4, PWM_FWD5, PWM_FWD6, PWM_FWD7};
int revSpeed[] = {PWM_FLT, PWM_REV1, PWM_REV2, 
    PWM_REV3, PWM_REV4, PWM_REV5, PWM_REV6, PWM_REV7};
int curSpeed = 0;

// IR led on port 13
LEGOPowerFunctions lego(13);
int potPin = A2;    // select the input pin for the potentiometer
int val = 0; 
int setSpeed = 0;
int ledPin[] = {2, 3, 4, 5, 6, 7, 8};
int buttonPin = 10;
int buttonState=0;
int fwdRev=0;

void setup() {
  for (int i=0; i<7; i++) {
    pinMode(ledPin[i], OUTPUT);
  }
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
}

void loop() {
  val = analogRead(potPin);    // read the value from the sensor
  Serial.print("POT: ");
  Serial.println(val);
  
  buttonState = digitalRead(buttonPin);
  Serial.print("BUTTON: ");
  Serial.println(buttonState);
  
if (buttonState) fwdRev = !fwdRev;  
  
setSpeed=val/125;
  if(setSpeed>7) setSpeed=7;
  
  for (int i=0; i<7; i++) {
      digitalWrite(ledPin[i], LOW);
  }
  
  for (i=0; i<setSpeed; i++) {
    digitalWrite(ledPin[i], HIGH);
  }
  
  Serial.print("SPEED: ");
  Serial.println(setSpeed);
  if (fwdRev==0) {
    curSpeed=fwdSpeed[setSpeed];
  } else {
    curSpeed=revSpeed[setSpeed];
  }
  lego.ComboPWM(curSpeed, curSpeed, CH1); // set speed
  delay(100);
  

}

