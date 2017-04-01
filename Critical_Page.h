#ifndef CRITICAL_PAGE_H
#define CRITICAL_PAGE_H

#include "Lcd_Controller.h"
enum Flag{
  LIMP,
  AERO,
  TC,
  REGEN,
  LV,
};

enum Numeric{
  SOC,
  TEMP,
  POWER
};

class Critical_Page {

  public:
    static void begin();
    static void setFlagLight(Flag field);
    static void setFlag(Flag field, bool state);
    static void setNumericLight(Numeric field);
    static void setNumeric(Numeric field, int8_t value);
    static void setNumeric(Numeric field, float value);
    static void display();


};

#endif
