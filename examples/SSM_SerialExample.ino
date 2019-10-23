/*
   SSM_SerialExample

   by: Daniel Nebert

   Creating a SSM object by passing an array of int representing
   the pin numbers connected to the displays select lines, the number
   of select lines used, the data direction register and PORT for
   the port connected to pins A through decimal point on the display.

   refresh(int duration) needs to be called constantly.

   Digits from the serial monitor can be displayed on the display12


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
const uint8_t characters[16] = { 0b11000000, //0
                                 0b11111001, //1
                                 0b10100100, //2
                                 0b10110000, //3
                                 0b10011001, //4
                                 0b10010010, //5
                                 0b10000010, //6
                                 0b11111000, //7
                                 0b10000000, //8
                                 0b10011000, //9
                               };




bool bDebug = true;

volatile uint8_t current[NUMBER_OF_SELECT_PINS] = { characters[1], characters[1], characters[1], characters[1] };

int selectPins[NUMBER_OF_SELECT_PINS] = { SELECT_PIN_1, SELECT_PIN_2, SELECT_PIN_3, SELECT_PIN_4 };

SevenSeg_Multiplexed ssm = SevenSeg_Multiplexed(selectPins, NUMBER_OF_SELECT_PINS, &DDR, &PORT);

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  debug("Serial comm. online");

  // Play display boot sequence
  debug("Starting boot sequence");
  ssm.playbootSeq();
  debug("Finished boot sequence");

  for (int i = 0; i < NUMBER_OF_SELECT_PINS; i++) {
    current[i] = characters[0];
  }
  ssm.updateDisplay(current);
}
String input;
void loop() {
  if (Serial.available() > 0) {
    
    // Capturing the input from console
    input = Serial.readStringUntil('\n');
    Serial.print("Received: ");
    Serial.println(input);
    char inputs[input.length()];
    input.toCharArray(inputs, input.length() + 1);
    int inputLength = sizeof(inputs);

    // Iterate over input and put digits in 'current'
    int selectPin = 0;
    for (int i = inputLength - NUMBER_OF_SELECT_PINS; i < inputLength; i++) {
      if(NUMBER_OF_SELECT_PINS - inputLength - 1 > i) {
        current[selectPin] = characters[0];
      } else {
        current[selectPin] = characters[inputs[i] - '0'];
      }
      selectPin++;
      
    }
    // Once we are done updating the values, update the ssm object
    ssm.updateDisplay(current);
  }
  // refresh
  ssm.refresh(3);
}

void debug(String output) {
  if (bDebug) {
    Serial.println(output);
  }
}
