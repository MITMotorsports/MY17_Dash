#include "Lcd_Controller.h"

//rs, enable, db4, db5, db6, db7
static LiquidCrystal lcd(67, 66, 65, 64, 63, 62);

void Lcd_Controller::begin(){
  lcd.begin(16, 2);
}

void Lcd_Controller::clearScreen(){
  lcd.clear();
  lcd.home();
}

void Lcd_Controller::writeMessage(String s, uint8_t col, uint8_t row) {
  lcd.setCursor(col, row);
  lcd.print(s);
}
