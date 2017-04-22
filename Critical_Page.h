#ifndef CRITICAL_PAGE_H
#define CRITICAL_PAGE_H

#include "Lcd_Controller.h"
enum Flag{
  LIMP,
  AERO,
  TC,
  REGEN,
  LV
};

enum Numeric{
  SOC,
  TEMP,
  POWER
};

enum Takeover{
  SHUTDOWNLOOP,
  BREAKTHROTTLE
};

class Critical_Page {
  public:
    static void begin();
    static void setFlag(Flag field, bool state);
    static void setNumeric(Numeric field, int8_t value);
    static void setNumeric(Numeric field, double value);
    static void setTakeover(Takeover field, bool state);
    static void display();
  private:
    static void setFlagLight(Flag field);
    static void setNumericLight(Numeric field);
    static void setTakeoverLight(Takeover field);
};

#endif
