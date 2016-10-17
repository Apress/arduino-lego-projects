#include <AFMotor.h>

// Connect a stepper motor with 48 steps per revolution (7.5 degree)
// to motor port #1 (M1 and M2)
AF_Stepper motor(48, 1);

int photocellPin = A0;     // the cell and 10K pulldown are connected to a0
int photocellReading;     // the analog reading from the sensor divider
int threshold = 200; // the amount of light required to activate the motor 

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps

  motor.setSpeed(50);  // 50 rpm   
}

void loop() {
    photocellReading = analogRead(photocellPin);  
 
  Serial.print("Photocell reading = ");
  Serial.println(photocellReading);     // the raw analog reading

  if (photocellReading > threshold) {
    motor.step(100, FORWARD, INTERLEAVE);  
  }

  
  delay(100);
}

