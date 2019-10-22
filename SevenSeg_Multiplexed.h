#include "Arduino.h"

class SevenSeg_Multiplexed
{
  public:  
    SevenSeg_Multiplexed(int *selPins, int c, volatile uint8_t *ddr, volatile uint8_t *port);
    int getSegCount();
    String toString();
    void playbootSeq();
    void allSelectOn();
    void allSelectOff();
    void refresh(int pulseDuration);
    void pulseSelectLine(int line, int pulseDuration);
    void updateDisplay(volatile uint8_t *digits);
  private:
    int count;
    int *pins;
    volatile uint8_t *segPort;
    volatile uint8_t *characters;
};