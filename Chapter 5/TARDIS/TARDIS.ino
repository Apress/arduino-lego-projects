#include "WaveHC.h"
#include "WaveUtil.h"

SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the volumes root directory
WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time

uint8_t dirLevel; // indent level for file/dir names    (for prettyprinting)
dir_t dirBuf;     // buffer for directory reads

//LED Variables
int ledvalue = 0;                            // variable to keep the actual value 
int ledpin = 6;                           // light connected to digital pin 6
int up=1;
int delaytime;


/*
 * Define macro to put error messages in flash memory
 */
#define error(msg) error_P(PSTR(msg))

// Function definitions (we define them here, but the code is below)
void play(FatReader &dir);

//////////////////////////////////// SETUP
void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps for debugging
  pinMode(6, OUTPUT);
  pinMode(17, OUTPUT);

  putstring_nl("\nWave test!");  // say we woke up!

  putstring("Free RAM: ");       // This can help with debugging, running out of RAM is bad
  Serial.println(FreeRam());

  //  if (!card.init(true)) { //play with 4 MHz spi if 8MHz isn't working for you
  if (!card.init()) {         //play with 8 MHz spi (default faster!)  
    error("Card init. failed!");  // Something went wrong, lets print out why
  }

  // enable optimize read - some cards may timeout. Disable if you're having problems
  card.partialBlockRead(true);

  // Now we will look for a FAT partition!
  uint8_t part;
  for (part = 0; part < 5; part++) {   // we have up to 5 slots to look in
    if (vol.init(card, part)) 
      break;                           // we found one, let’s bail
  }
  if (part == 5) {                     // if we ended up not finding one  :(
    error("No valid FAT partition!");  // Something went wrong, lets print out why
  }

  // Lets tell the user about what we found
  putstring("Using partition ");
  Serial.print(part, DEC);
  putstring(", type is FAT");
  Serial.println(vol.fatType(),DEC);     // FAT16 or FAT32?

  // Try to open the root directory
  if (!root.openRoot(vol)) {
    error("Can't open root dir!");      // Something went wrong,
  }

  // Whew! We got past the tough parts.
  putstring_nl("Files found (* = fragmented):");

  // Print out all of the files in all the directories.
  root.ls(LS_R | LS_FLAG_FRAGMENTED);

}

//////////////////////////////////// LOOP
void loop() { 

  delay(500);
  root.rewind();
  play(root);
 ledvalue=0;
analogWrite(ledpin, ledvalue);

  delayTime=7 * 60 * 1000; //7 minutes * 60 seconds * 1000 milliseconds = 420000
  delay(delayTime);
  Serial.println("loopit");
}

/////////////////////////////////// HELPERS
/*
 * print error message and halt
 */
void error_P(const char *str) {
  PgmPrint("Error: ");
  SerialPrint_P(str);
  sdErrorCheck();
  while(1);
}
/*
 * print error message and halt if SD I/O error, great for debugging!
 */
void sdErrorCheck(void) {
  if (!card.errorCode()) return;
  PgmPrint("\r\nSD I/O error: ");
  Serial.print(card.errorCode(), HEX);
  PgmPrint(", ");
  Serial.println(card.errorData(), HEX);
  while(1);
}
/*
 * play recursively - possible stack overflow if subdirectories too nested
 */
void play(FatReader &dir) {

  FatReader file;
  while (dir.readDir(dirBuf) > 0) {    // Read every file in the directory one at a time

    // Skip it if not a subdirectory and not a WAV file
    if (!DIR_IS_SUBDIR(dirBuf)
      && strncmp_P((char *)&dirBuf.name[8], PSTR("WAV"), 3)) {
      continue;
    }

    Serial.println();            // clear out a new line

    for (uint8_t i = 0; i < dirLevel; i++) {
      Serial.print(' ');       // this is for prettyprinting, put spaces in front
    }
    if (!file.open(vol, dirBuf)) {        // open the file in the directory
      error("file.open failed");          // something went wrong
    }

    if (file.isDir()) {                   // check if we opened a new directory
      putstring("Subdir: ");
      printEntryName(dirBuf);
      dirLevel += 2;                      // add more spaces
      // play files in subdirectory
      play(file);                         // recursive!
      dirLevel -= 2;    
    } else {
      // Aha! we found a file that isn’t a directory
      putstring("Playing ");
      printEntryName(dirBuf);              // print it out
      if (!wave.create(file)) {            // Figure out, is it a WAV proper?
        putstring(" Not a valid WAV");     // ok skip it
      } else {
        Serial.println();                  // Hooray it IS a WAV proper!
        wave.play();                       // make some noise!

        uint8_t n = 0;
        while (wave.isplaying) {// playing occurs in interrupts, so we print dots in realtime
          if (up == 1) {
            ledvalue=ledvalue+3; //Gradually increase the illumination if we are lighting up
          } else {
             ledvalue=ledvalue-3;  //Otherwise decrease to lower the lighting
          }
          
          if (ledvalue > 255) {  //If we reach maximum illumination, start decreasing
            up=0;
            ledvalue=ledvalue-3;
          } else if (ledvalue < 0) { //Otherwise we’re going to make the light brighter
             up=1;
             ledvalue=ledvalue+3;
          }
          analogWrite(ledpin, ledvalue);

          putstring(".");
          if (!(++n % 32)) Serial.println();
          delay(100);


        }       
        sdErrorCheck();                    // everything OK?
        // if (wave.errors)Serial.println(wave.errors);     // wave decoding errors
      }
    }
  }
}

