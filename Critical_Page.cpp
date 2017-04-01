#include "Critical_Page.h"
#include <Arduino.h>

bool displayed = false;
bool limpOn = false;
bool aeroOn = false;
bool tcOn = false;
bool regenOn = false;
bool lvLow = false;
bool soc = 0 ;
bool temp = 0;
bool power = 00.0;

void Critical_Page::begin(){
}
void Critical_Page::setFlag(Flag field, bool state){
  //checks if you're trying to set it with a bool
  switch(field) {
    case LIMP:
      limpOn = state;
      if (displayed){
        setFlagLight(LIMP);
      }
      break;
    case AERO:
      aeroOn = state;
      if (displayed){
        setFlagLight(AERO);
      }
      break;
    case TC:
      tcOn = state;
      if (displayed){
        setFlagLight(TC);
      }
      break;
    case REGEN:
      regenOn = state;
      if (displayed){
        setFlagLight(REGEN);
      }
      break;
    case LV:
      lvLow = state;
      if (displayed){
        setFlagLight(LV);
      }
      break;
  }
}

void Critical_Page::setFlagLight(Flag field){
  switch(field) {
    case LIMP:
      Lcd_Controller::writeMessage(limpOn,0,0);
    case AERO:
      Lcd_Controller::writeMessage(aeroOn,5,0);
    case TC:
      Lcd_Controller::writeMessage(tcOn,8,0);
    case REGEN:
      Lcd_Controller::writeMessage(regenOn,11,0);
    case LV:
      Lcd_Controller::writeMessage(lvLow,14,0);
  }
}


void Critical_Page::setNumericLight(Numeric field){
  switch(field) {
    case SOC:
      Lcd_Controller::writeMessage(soc,3,1);
      break;
    case TEMP:
      Lcd_Controller::writeMessage(temp,6,1);
      break;
    case POWER:
      Lcd_Controller::writeMessage(power,10,1);
      break;
  }
}

void Critical_Page::setNumeric(Numeric field, int8_t value){
  switch(field) {
    case SOC:
      soc = value;
      if (displayed){
        setNumericLight(SOC);
      }
      break;
    case TEMP:
      temp = value;
      if (displayed){
        setNumericLight(TEMP);
      }
      break;
    case POWER:
      power = value;
      if (displayed){
        setNumericLight(POWER);
      }
      break;
  }
}

void Critical_Page::setNumeric(Numeric field, float value){
  switch(field) {
    case SOC:
      soc = value;
      if (displayed){
        setNumericLight(SOC);
      }
      break;
    case TEMP:
      temp = value;
      if (displayed){
        setNumericLight(TEMP);
      }
      break;
    case POWER:
      power = value;
      if (displayed){
        setNumericLight(POWER);
      }
      break;
  }
}

void Critical_Page::display(){
  //set the displayed flag to be true
  displayed = true;
  //always clear the previous screem
  Lcd_Controller::clearScreen();
  //initialize the display with things that never change
  Lcd_Controller::writeMessage("SOC     C     KW",0,0);
  setFlagLight(LIMP);
  setFlagLight(AERO);
  setFlagLight(TC);
  setFlagLight(REGEN);
  setFlagLight(LV);
  setNumericLight(SOC);
  setNumericLight(TEMP);
  setNumericLight(POWER);
}
