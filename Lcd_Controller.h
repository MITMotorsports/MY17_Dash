#ifndef LCD_CONTROLLER_H
#define LCD_CONTROLLER_H

#include <Wire.h>
#include <LiquidCrystal.h>

//rs, enable, db4, db5, db6, db7
static LiquidCrystal lcd(67, 66, 65, 64, 63, 62);

class Lcd_Controller{
  public:
    static void begin();
    template <class T>
    static void writeMessage(T message, uint8_t col, uint8_t row){
      lcd.setCursor(col, row);
      lcd.print(message);
    };
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
