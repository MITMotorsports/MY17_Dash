#include "Lcd_Controller.h"


void Lcd_Controller::begin(){
  lcd.begin(16, 2);
}

void Lcd_Controller::clearScreen(){
  lcd.clear();
}
