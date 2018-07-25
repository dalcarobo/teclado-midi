// Pin Definitions
// Rows are connected to
const int row1 = 2;
const int row2 = 3;
const int row3 = 4;
const int row4 = 5;
const int row5 = 6;
const int row6 = 7;
const int row7 = 8;
const int row8 = 9;

// The 74HC595 uses a serial communication 
// link which has three pins
const int clock = 12;
const int latch = 11;
const int data = 10;


uint8_t keyToMidiMap[64];

boolean keyPressed[64];

int noteVelocity = 127;


// use prepared bit vectors instead of shifting bit left everytime
int bits[] = { B11111110,
  B11111101,
  B11111011,
  B11110111,
  B11101111,
  B11011111,
  B10111111,
  B01111111
  };


// 74HC595 shift to next column
void scanColumn(int value) {
  digitalWrite(latch, LOW); //Pulls the chips latch low
  shiftOut(data, clock, MSBFIRST, value); //Shifts out the 8 bits to the shift register
  digitalWrite(latch, HIGH); //Pulls the latch high displaying the data
}

void setup() {
  
  // Map scan matrix buttons/keys to actual Midi note number. Lowest num 41 corresponds to F MIDI note.
/*  keyToMidiMap[0] = 43;
  keyToMidiMap[1] = 36;
  keyToMidiMap[2] = 37;
  keyToMidiMap[3] = 38;
  keyToMidiMap[4] = 39;
  keyToMidiMap[5] = 40;
  keyToMidiMap[6] = 41;
  keyToMidiMap[7] = 42;
*/

  keyToMidiMap[0] = 43;
  keyToMidiMap[1] = 42;
  keyToMidiMap[2] = 41;
  keyToMidiMap[3] = 40;
  keyToMidiMap[4] = 39;
  keyToMidiMap[5] = 38;
  keyToMidiMap[6] = 37;
  keyToMidiMap[7] = 36;


  keyToMidiMap[8]     = 51;
  keyToMidiMap[1 + 8] = 50;
  keyToMidiMap[2 + 8] = 49;
  keyToMidiMap[3 + 8] = 48;
  keyToMidiMap[4 + 8] = 47;
  keyToMidiMap[5 + 8] = 46;
  keyToMidiMap[6 + 8] = 45;
  keyToMidiMap[7 + 8] = 44;

  keyToMidiMap[16]     = 59;
  keyToMidiMap[1 + 16] = 58;
  keyToMidiMap[2 + 16] = 57;
  keyToMidiMap[3 + 16] = 56;
  keyToMidiMap[4 + 16] = 55;
  keyToMidiMap[5 + 16] = 54;
  keyToMidiMap[6 + 16] = 53;
  keyToMidiMap[7 + 16] = 52;

  keyToMidiMap[24] = 67;
  keyToMidiMap[1 + 24] = 66;
  keyToMidiMap[2 + 24] = 65;
  keyToMidiMap[3 + 24] = 64;
  keyToMidiMap[4 + 24] = 63;
  keyToMidiMap[5 + 24] = 62;
  keyToMidiMap[6 + 24] = 61;
  keyToMidiMap[7 + 24] = 60;

  keyToMidiMap[32] = 75;
  keyToMidiMap[1 + 32] = 74;
  keyToMidiMap[2 + 32] = 73;
  keyToMidiMap[3 + 32] = 72;
  keyToMidiMap[4 + 32] = 71;
  keyToMidiMap[5 + 32] = 70;
  keyToMidiMap[6 + 32] = 69;
  keyToMidiMap[7 + 32] = 68;

  keyToMidiMap[40] = 83;
  keyToMidiMap[1 + 40] = 82;
  keyToMidiMap[2 + 40] = 81;
  keyToMidiMap[3 + 40] = 80;
  keyToMidiMap[4 + 40] = 79;
  keyToMidiMap[5 + 40] = 78;
  keyToMidiMap[6 + 40] = 77;
  keyToMidiMap[7 + 40] = 76;

  keyToMidiMap[48] = 91;
  keyToMidiMap[1 + 48] = 90;
  keyToMidiMap[2 + 48] = 89;
  keyToMidiMap[3 + 48] = 88;
  keyToMidiMap[4 + 48] = 87;
  keyToMidiMap[5 + 48] = 86;
  keyToMidiMap[6 + 48] = 85;
  keyToMidiMap[7 + 48] = 84;

  keyToMidiMap[56] = 99;
  keyToMidiMap[1 + 56] = 98;
  keyToMidiMap[2 + 56] = 97;
  keyToMidiMap[3 + 56] = 96;
  keyToMidiMap[4 + 56] = 95;
  keyToMidiMap[5 + 56] = 94;
  keyToMidiMap[6 + 56] = 93;
  keyToMidiMap[7 + 56] = 92;
  
  // setup pins output/input mode
  pinMode(data, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(latch, OUTPUT);

  pinMode(row1, INPUT);
  pinMode(row2, INPUT);
  pinMode(row3, INPUT);
  pinMode(row4, INPUT);
  pinMode(row5, INPUT);
  pinMode(row6, INPUT);
  pinMode(row7, INPUT);
  pinMode(row8, INPUT);

    Serial.begin(9600);

  delay(10);

}

void loop() {

  for (int col = 0; col < 8; col++) {
    
    // shift scan matrix to following column
    scanColumn(bits[col]);

    // check if any keys were pressed - rows will have HIGH output in this case corresponding
    int groupValue1 = !digitalRead(row1);
    int groupValue2 = !digitalRead(row2);
    int groupValue3 = !digitalRead(row3);
    int groupValue4 = !digitalRead(row4);
    int groupValue5 = !digitalRead(row5);
    int groupValue6 = !digitalRead(row6);
    int groupValue7 = !digitalRead(row7);
    int groupValue8 = !digitalRead(row8);

    // process if any combination of keys pressed
    if (groupValue1 != 0 || groupValue2 != 0 || groupValue3 != 0
        || groupValue4 != 0  || groupValue5 != 0  || groupValue6 != 0  || groupValue7 != 0  || groupValue8 != 0) {

      if (groupValue1 != 0 && !keyPressed[col]) {
        keyPressed[col] = true;
        noteOn(0x91, keyToMidiMap[col], noteVelocity);
      }

      if (groupValue2 != 0 && !keyPressed[col + 8]) {
        keyPressed[col + 8] = true;
        noteOn(0x91, keyToMidiMap[col + 8], noteVelocity);
      }

      if (groupValue3 != 0 && !keyPressed[col + 16]) {
        keyPressed[col + 16] = true;
        noteOn(0x91, keyToMidiMap[col + 16], noteVelocity);
      }

      if (groupValue4 != 0 && !keyPressed[col + 24]) {
        keyPressed[col + 24] = true;
        noteOn(0x91, keyToMidiMap[col + 24], noteVelocity);
      }

      if (groupValue5 != 0 && !keyPressed[col + 32]) {
        keyPressed[col + 32] = true;
        noteOn(0x91, keyToMidiMap[col + 32], noteVelocity);
      }

      if (groupValue6 != 0 && !keyPressed[col + 40]) {
        keyPressed[col + 40] = true;
        noteOn(0x91, keyToMidiMap[col + 40], noteVelocity);
      }

      if (groupValue7 != 0 && !keyPressed[col + 48]) {
        keyPressed[col + 48] = true;
        noteOn(0x91, keyToMidiMap[col + 48], noteVelocity);
      }

      if (groupValue8 != 0 && !keyPressed[col + 56]) {
        keyPressed[col + 56] = true;
        noteOn(0x91, keyToMidiMap[col + 56], noteVelocity);
      }

    }

    //  process if any combination of keys released
    if (groupValue1 == 0 && keyPressed[col]) {
      keyPressed[col] = false;
      noteOn(0x91, keyToMidiMap[col], 0);
    }

    if (groupValue2 == 0 && keyPressed[col + 8]) {
      keyPressed[col + 8] = false;
      noteOn(0x91, keyToMidiMap[col + 8], 0);
    }

    if (groupValue3 == 0 && keyPressed[col + 16]) {
      keyPressed[col + 16] = false;
      noteOn(0x91, keyToMidiMap[col + 16], 0);
    }

    if (groupValue4 == 0 && keyPressed[col + 24]) {
      keyPressed[col + 24] = false;
      noteOn(0x91, keyToMidiMap[col + 24], 0);
    }

 if (groupValue5 == 0 && keyPressed[col + 32]) {
      keyPressed[col + 32] = false;
      noteOn(0x91, keyToMidiMap[col + 32], 0);
    }
     if (groupValue6 == 0 && keyPressed[col + 40]) {
      keyPressed[col + 40] = false;
      noteOn(0x91, keyToMidiMap[col + 40], 0);
    }
     if (groupValue7 == 0 && keyPressed[col + 48]) {
      keyPressed[col + 48] = false;
      noteOn(0x91, keyToMidiMap[col + 48], 0);
    }
     if (groupValue8 == 0 && keyPressed[col + 56]) {
      keyPressed[col + 56] = false;
      noteOn(0x91, keyToMidiMap[col + 56], 0);
    }
  }

}


void noteOn(int cmd, int pitch, int velocity) {
    Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}
