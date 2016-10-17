// sets the constants for the sensor and led signal pins:
const int pingPin = 2;
const int led = 10;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  
  // sets the LED pin to an output mode
  pinMode(led, OUTPUT);
}

void loop() {
  // establish variables for duration of the ping, 
  // and the distance result in inches:
  long duration, inches;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  
  // turn on the led if object is within six inches
  if (inches < 6) {
     digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }
  
  // send the value in inches to the Serial Monitor
  Serial.print(inches);
  Serial.println(" inches");
  
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

