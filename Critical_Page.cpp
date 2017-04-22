#include "Critical_Page.h"
#include <Arduino.h>

bool displayed = false;
bool limpOn = false;
bool aeroOn = false;
bool tcOn = false;
bool regenOn = false;
bool lvLow = false;
int8_t soc = 0 ;
int8_t temp = 0;
double power = 00.0;
bool shutdownLoop = false;
bool breakThrottle = false;

void Critical_Page::begin(){
}
void Critical_Page::setFlag(Flag field, bool state){
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
      if (limpOn){
        Lcd_Controller::writeMessage("LIMP",0,0);
      }
      if (!limpOn){
        Lcd_Controller::writeMessage("    ",0,0);
      }
    case AERO:
      if (aeroOn){
        Lcd_Controller::writeMessage("AE",5,0);
      }
      if (!aeroOn){
        Lcd_Controller::writeMessage("  ",5,0);
      }
    case TC:
      if (tcOn){
        Lcd_Controller::writeMessage("TC",8,0);
      }
      if (!tcOn){
        Lcd_Controller::writeMessage("  ",8,0);
      }
    case REGEN:
      if (regenOn){
        Lcd_Controller::writeMessage("RG",11,0);
      }
      if (!regenOn){
        Lcd_Controller::writeMessage("  ",11,0);
      }
    case LV:
      if (lvLow){
        Lcd_Controller::writeMessage("LV",14,0);
      }
      if (!lvLow){
        Lcd_Controller::writeMessage("  ",14,0);
      }
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

void Critical_Page::setTakeoverLight(Takeover field){
  switch(field){
    case SHUTDOWNLOOP:
      Lcd_Controller::clearScreen();
      Lcd_Controller::writeMessage("SHUTDOWN",0,0);
      Lcd_Controller::writeMessage("LOOP",0,1);
      break;
    case BREAKTHROTTLE:
      Lcd_Controller::clearScreen();
      Lcd_Controller::writeMessage("BREAK",0,0);
      Lcd_Controller::writeMessage("THROTTLE",0,1);
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

void Critical_Page::setNumeric(Numeric field, double value){
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

void Critical_Page::setTakeover(Takeover field, bool state){
  switch(field){
    case SHUTDOWNLOOP:
      shutdownLoop = state;
      if (displayed){
        setTakeoverLight(SHUTDOWNLOOP);
      };
      break;
    case BREAKTHROTTLE:
      breakThrottle = state;
      if (displayed){
        setTakeoverLight(BREAKTHROTTLE);
      };
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
