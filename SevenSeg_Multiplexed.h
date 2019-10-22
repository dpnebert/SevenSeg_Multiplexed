/*
 * SSM_SimpleExample.h
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
	20191022 - DN:
		Moved
			void pulseSelectLine(int line, int pulseDuration);
		from public to private.
*/

#include "Arduino.h"

class SevenSeg_Multiplexed
{
  public:
  
	// Int array selPins for select pin-pin numbers
	// The count of select pins used
	// The data direction register for our output pins
	// The actual port used as a bus for the 8 segment LEDs
    SevenSeg_Multiplexed(int *selPins, int c, volatile uint8_t *ddr, volatile uint8_t *port);
	
	// Ability to turn on/off all select line
    void allSelectOn();
    void allSelectOff();
	
	// Call refresh in main loop to pulse all select lines with appriopriate characters displayed
    void refresh(int pulseDuration);
	
	// Updates the characters used on display
    void updateDisplay(volatile uint8_t *digits);
	
  private:
  
	// Gotta have a bit of flair.  Get it?  bit? ;)
    void playbootSeq();
	
	// Internal stuff used by interal stuff, internally
    void pulseSelectLine(int line, int pulseDuration);
    int count;
    int *pins;
    volatile uint8_t *segPort;
    volatile uint8_t *characters;
};