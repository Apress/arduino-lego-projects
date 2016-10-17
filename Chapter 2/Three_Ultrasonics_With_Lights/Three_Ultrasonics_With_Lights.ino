// sets the constants for each of the sensor and led signal pins:
const int pingPin[] = {2, 3, 4};
const int led[] = {10, 11, 12};

// sets the increment counter for each sensor:
 int counter = 0;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  
  // sets each LED pin to an output mode
  for (int i=0; i<3; i++) pinMode(led[i], OUTPUT);
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
  
  // turn on the led if object is within six inches
  if (inches < 6) {
     digitalWrite(led[counter], HIGH);
  } else {
    digitalWrite(led[counter], LOW);
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

