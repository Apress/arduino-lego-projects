// include the library for hobby servos
#include <Servo.h>

// DC hobby servo
Servo servo1;

// sets the constants for each of the sensor signal pins:
const int pingPin[] = {2, 3, 4};

// sets the increment counter for each sensor:
int counter = 0;

// sets the speed of the servo movement
int spd = 10;

// sets the left, right, and center positions of the servo
int left = 10;
int right = 170;
int center = (right - left) / 2;

// sets the variable to keep track of the servo angle
int angle = center;
 
void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  
  // turn on servo and move to center
  servo1.attach(9);
  servo1.write(center);
}

void loop() {
  // establish variables for duration of the ping, 
  // and the distance result in inches:
  long duration, inches;
  
  // resets counter if we run out of sensors
  if (counter == 3) counter = 0;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin[counter], OUTPUT);
  digitalWrite(pingPin[counter], LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin[counter], HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin[counter], LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin[counter], INPUT);
  duration = pulseIn(pingPin[counter], HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  
  // moves the servo to the left if left sensor is triggered
  if (inches < 6 && counter == 0) {
    if (angle != left) {
      for (int i=angle; i>left; i--) {
        servo1.write(i);
        delay(spd);
      }
      angle = left;
    }
    
  // moves to the center if center sensor is triggered
  } else if (inches < 6 && counter == 1) {
    // moves from left to center
    if (angle < center) {
      for (int i=angle; i<center; i++) {
        servo1.write(i);
        delay(spd);
      }
    // or moves from right to center
    } else {
      for (int i=angle; i>center; i--) {
        servo1.write(i);
        delay(spd);
      }
    }
    angle = center;
    
  // moves to the right if right sensor is triggered
  } else if (inches < 6 && counter == 2) {
    if (angle != right) {
      for (int i=angle; i<right; i++) {
        servo1.write(i);
        delay(spd);
      }
      angle = right;
    }
    
  // otherwise hold steady at the current position
  } else {
    servo1.write(angle);
  }
  
  // send the value in inches to the Serial Monitor for each sensor
  Serial.print("Sensor ");
  Serial.print(counter);
  Serial.print(": ");
  Serial.print(inches);
  Serial.println(" inches");
  
  // increment counter for the next loop
  counter++;
  
  // short delay before starting over again
  delay(100);
}

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}


