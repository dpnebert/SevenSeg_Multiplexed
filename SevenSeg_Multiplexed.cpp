/*
 * SSM_SimpleExample.cpp
 * 
 * by: Daniel Nebert
 * 
 * Creating a SSM object by passing an array of int representing
 * the pin numbers connected to the displays select lines, the number
 * of select lines used, the data direction register and PORT for
 * the port connected to pins A through decimal point on the display.
 * 
 * refresh(int duration) needs to be called constantly.
 * 
 * To change the values viewed on the screen, call updateDisplay,
 * and pass it a int array 'current' with bits set to create desired
 * character. Binary value: 0b11111111 would mean all off and 0b00000000
 * would mean all on.  If the bit is 1, the LED is off, else the LED is on.
 * 
 */

/*
	Changelog:
	
	20191022 - DN:
		Added comments and moved things around to make it look better
*/



#include "Arduino.h"
#include "SevenSeg_Multiplexed.h"

SevenSeg_Multiplexed::SevenSeg_Multiplexed(int* selPins, int c, volatile uint8_t *ddr, volatile uint8_t *port) {
  // Capture array of pins and the count
  pins = selPins;
  count = c;
  
  // Set out data direction register as output
  *ddr = 0xFF;
  // Capture PORT address, and set all bits to 1's
  segPort = port;
  *segPort = 0xFF;

  // initialize select pins as output, and turn them off.
  for(int i = 0; i < count; i++) {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], LOW);
  }
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
