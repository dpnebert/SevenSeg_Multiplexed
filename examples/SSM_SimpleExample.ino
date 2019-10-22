#include <SevenSeg_Multiplexed.h>

#define NUMBER_OF_SELECT_PINS 4
#define SELECT_PIN_1 50
#define SELECT_PIN_2 51
#define SELECT_PIN_3 52
#define SELECT_PIN_4 53

#define COUNT_LIMIT 50

#define DDR   DDRC
#define PORT  PORTC

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

void setup() {
  Serial.begin(9600);
  while(!Serial) {}
  debug("Serial comm. online");

  // Initializing 

  
  debug("Select pins: ");
  debug(ssm.toString());

  // Play display boot sequence
  debug("Starting boot sequence");
  ssm.playbootSeq();
  debug("Finished boot sequence");

}

int placekeeper = 0;
int count_limit = 3000;
void loop() {

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
  }
  count_limit++;

  // Pass new digits to ssm and refresh
  ssm.updateDisplay(current);
  ssm.refresh(3);  
}

void debug(String output) {
  if(bDebug) {
    Serial.println(output);
  }
}
