#ifndef LCD_CONTROLLER_H
#define LCD_CONTROLLER_H

#include <Wire.h>
#include <LiquidCrystal.h>

class Lcd_Controller{
  public:
    static void begin();
    static void writeMessage(String message, uint8_t col, uint8_t row);
    static void clearScreen();
};
#endif

// static void staticLcdUpdate(Flag type, LcdLightState state);
//
// template <class N>
// static void dynamicLcdUpdate(Numeric type, N value);
//
// static void takeoverLcdUpdate(Takeover type);
//write a message at the location (col, row)
