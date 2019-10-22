#include "Arduino.h"
#include "SevenSeg_Multiplexed.h"

SevenSeg_Multiplexed::SevenSeg_Multiplexed(int* selPins, int c, volatile uint8_t *ddr, volatile uint8_t *port) {
  // Capture array of pins and the count
  pins = selPins;
  count = c;
  *ddr = 0xFF;
  segPort = port;
  *segPort = 0xFF;

  // initialize select pins as output, and turn them off.
  for(int i = 0; i < count; i++) {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], LOW);
  }
}
int SevenSeg_Multiplexed::getSegCount() {
  return count;
}
String SevenSeg_Multiplexed::toString() {
  if(sizeof(pins) > 0) {
    String output = String(pins[0]);
    for(int i = 1; i < count; i++) {
      output.concat(", ");
      output.concat(String(pins[i]));
    }
    return output;
  }
  return "null";
}
void SevenSeg_Multiplexed::allSelectOn() {
  if(sizeof(pins) > 0) {
    for(int i = 0; i < count; i++) {
      digitalWrite(pins[i], HIGH);
    }
  }
}
void SevenSeg_Multiplexed::allSelectOff() {
  if(sizeof(pins) > 0) {
    for(int i = 0; i < count; i++) {
      digitalWrite(pins[i], LOW);
    }
  }
}
void SevenSeg_Multiplexed::updateDisplay(volatile uint8_t *digits) {
  characters = digits;
  for(int i = 0; i < count; i++) {
    Serial.println(String(characters[i]));
  }
}
void SevenSeg_Multiplexed::refresh(int pulseDuration) {
  for(int i = 0; i < count; i++) {
    *segPort = characters[i];
    pulseSelectLine(pins[i], pulseDuration);
  }
}
void SevenSeg_Multiplexed::pulseSelectLine(int line, int pulseDuration) {
  digitalWrite(line, HIGH);
  delay(pulseDuration);
  digitalWrite(line, LOW);
}
void SevenSeg_Multiplexed::playbootSeq() {
  if(sizeof(pins) > 0) {
    allSelectOn();
    
    for(int i = 8; i >= 0; i--) {
      *segPort = pow(2, i);
      delay(250);
    }
    for(int i = 1; i < 8; i++) {
      *segPort = pow(2, i);
      delay(250);
    }
    *segPort = 255;
    allSelectOff();
  }
}
