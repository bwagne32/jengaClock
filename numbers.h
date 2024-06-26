#include <stdint.h>
#ifndef numbers_h
#define numbers_h

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <vector>


std::vector<bool> convertStates(short digit) {
  switch (digit) {
    case (1):
      return {0,1,1,0,0,0,0};
      break;
    case (2):
      return {1,1,0,1,1,0,1};
      break;
    case (3):
      return {1,1,1,1,0,0,1};
      break;
    case (4):
      return {0,1,1,0,0,1,1};
      break;
    case (5):
      return {1,0,1,1,0,1,1};
      break;
    case (6):
      return {1,0,1,1,1,1,1};
      break;
    case (7):
      return {1,1,1,0,0,0,0};
      break;
    case (8):
      return {1,1,1,1,1,1,1};
      break;
    case (9):
      return {1,1,1,1,0,1,1};
      break;
    case (0):
      return {1,1,1,1,1,1,0};
      break;
    default:
      return {0,0,0,0,0,0,0}; // turns off display for seg 1
  }
}

void output(Adafruit_PWMServoDriver& board, uint8_t start, uint8_t end, std::vector<bool> states, std::vector<uint16_t> onVals, std::vector<uint16_t> offVals) {
  
  for(int i = start,j = 0; i <= end; i++,j++){
    if(states[j])
      board.setPWM(i, 0, onVals[j]);
    else
      board.setPWM(i, 0, offVals[j]);
  }
}


#endif