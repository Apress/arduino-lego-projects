#include <SoftwareSerial.h>

SoftwareSerial rfid(2,3);

int ledpin[] = {3, 5, 6, 9, 10, 11};                        
int ledcount=6; 

char tag01[] = "4500B8F08489";
char tag02[] = "4500B8D36947";

char tagString[13];

void setup() {
  Serial.begin(9600);
  rfid.begin(9600);
}

void loop() {
  if (rfid.available()) {
    if (getTag()) {
        printTag();
        for (int numval=0; numval < 4; numval++) {
            for(int value = 255; value >=100; value=value-5) {   // fade out (from max to min) 
                for (int i=0; i<ledcount; i++) {
                  analogWrite(ledpin[i], value);          // sets the value (range from 0 to 255) 
                }  
                delay(30);
            }
           delay(500);
           for(int value = 100 ; value <= 255; value=value+5) { // fade in (from min to max)  
               for (int i=0; i<ledcount; i++) {
                   analogWrite(ledpin[i], value);          // sets the value (range from 0 to 255) 
               }
               delay(30);                            // waits for 30 milli seconds to see the dimming effect 
           }
           delay(500);
        }
        
        for(int value = 255; value >=0; value=value-3) {  // fade out (from max to min)  
            for (int i=0; i<ledcount; i++) {
                analogWrite(ledpin[i], value);          // sets the value (range from 0 to 255) 
            }
           delay(30); 
        }
      
        for (int i=0; i<ledcount; i++) {
            analogWrite(ledpin[i], 0);          // sets the value (range from 0 to 255) 
        }

    }
  }
}

boolean getTag() {
  char startByte = rfid.read();
  delay(20);
    
  if (startByte == 2) {
    int index = 0;
    while (index < 12) {
      char incomingByte = rfid.read();

      tagString[index] = incomingByte;   
      index++;
    }
  }
  rfid.flush();
  return true;
}

void printTag() {    
  for (int i=0; i<12; i++) Serial.print(tagString[i]);
  Serial.println(compareTags());
}

const char* compareTags() {
  if (strncmp(tag01, tagString, 12) == 0) return " Tag 1";
  else if (strncmp(tag02, tagString, 12) == 0) return " Tag 2";
  else return " Not recognized.";
}

