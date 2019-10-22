/*
 * SSM_SimpleExample
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


#include <SevenSeg_Multiplexed.h>

#define NUMBER_OF_SELECT_PINS   4
#define SELECT_PIN_1            50
#define SELECT_PIN_2            51
#define SELECT_PIN_3            52
#define SELECT_PIN_4            53
#define COUNT_LIMIT             50
#define DDR                     DDRC
#define PORT                    PORTC


                          //   D
                          //   pGFEDCBA
const uint8_t characters[16]= { 0b11000000, //0
                             0b11111001, //1
                             0b10100100, //2
                             0b10110000, //3
                             0b10011001, //4
                             0b10010010, //5
                             0b10000010, //6
                             0b11111000, //7
                             0b10000000, //8
                             0b10011000, //9
                             0b10001000, //A
                             0b10000011, //B
                             0b11000110, //C
                             0b10100001, //D
                             0b10000110, //E
                             0b10001110  //F
                           };




bool bDebug = true;

volatile uint8_t current[NUMBER_OF_SELECT_PINS] = { characters[1], characters[1], characters[1], characters[1] };

int selectPins[NUMBER_OF_SELECT_PINS] = { SELECT_PIN_1, SELECT_PIN_2, SELECT_PIN_3, SELECT_PIN_4 };

SevenSeg_Multiplexed ssm = SevenSeg_Multiplexed(selectPins, NUMBER_OF_SELECT_PINS, &DDR, &PORT);

int placekeeper = 0;
int count_limit = 3000;

void setup() {
  Serial.begin(9600);
  while(!Serial) {}
  debug("Serial comm. online");
    
  debug("Select pins: ");

  // Play display boot sequence
  debug("Starting boot sequence");
  ssm.playbootSeq();
  debug("Finished boot sequence");

}

void loop() {

  // Simply waits until it's counted up to COUNT_LIMIT
  // before changin all four displays to the same character
  //
  // For demo purposes.  To make a timer, a internal interrupt
  // should be used to trigger an increment.  Or just call
  // updateDisplay when a value changes.  Just be sure to call
  // refresh as often as possible to reduce flicker
  if(count_limit >= COUNT_LIMIT) {
    count_limit = 0;
    current[0] = characters[placekeeper];
    current[1] = characters[placekeeper];
    current[2] = characters[placekeeper];
    current[3] = characters[placekeeper];
    if(placekeeper >= 16) {
      placekeeper = 0;
    } else {
      placekeeper++;
    }

    // Now we have new value to display, call updateDisplay
    ssm.updateDisplay(current);
  }
  count_limit++;

  // refresh
  ssm.refresh(3);  
}

void debug(String output) {
  if(bDebug) {
    Serial.println(output);
  }
}
