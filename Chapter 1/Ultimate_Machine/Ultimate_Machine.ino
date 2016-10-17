#include <Servo.h> 

// DC hobby servo
Servo servo1;

// Switch connected to digital pin 2
int SwitchPin = 2;  

void setup() {
  // turn on servo
  servo1.attach(9);
  
  // sets the digital pin 2 as input
  // and enables pullup resistor
  pinMode(SwitchPin, INPUT_PULLUP);       
}

void loop() {
  // read the input pin
  int val = digitalRead(SwitchPin);   
  
  // test if switch has been triggered
  if (val == LOW) {
    servo1.write(115);
    delay(250); 
    servo1.write(0);
  }
  delay(100);
} 

