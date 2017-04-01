#include "Lcd_Controller.h"


void Lcd_Controller::begin(){
  lcd.begin(16, 2);
}

void Lcd_Controller::clearScreen(){
  lcd.clear();
}
// void Lcd_Controller::staticLcdUpdate(Flag type, LcdLightState state){
//   if (state == LCDON){
//     switch(type){
//       case LIMP:
//         writeMessage("LIMP", 0,0);
//         break;
//       case AE:
//         writeMessage("AE",5, 0);
//         break;
//       case TC:
//         writeMessage("AE", 8,0);
//         break;
//       case RG:
//         writeMessage("RG", 11, 0);
//         break;
//       case LV:
//         writeMessage("LV", 14, 0);
//         break;
//     }
//   }
// }
//
// template <class N>
// void Lcd_Controller::dynamicLcdUpdate(Numeric type, N value){
//   switch(type){
//     case SOC:
//       writeMessage(value, 3, 1);
//       break;
//     case TEMP:
//       writeMessage(value, 6, 1);
//       break;
//     case POWER:
//       writeMessage(value, 10, 1);
//       break;
//   }
// }
//
// void Lcd_Controller::takeoverLcdUpdate(Takeover type){
//   switch(type){
//     case SDLOOP:
//       writeMessage("SHUTDOWN",4,0);
//       writeMessage("LOOP",6,1);
//       break;
//     case BREAKTHROTTLE:
//       writeMessage("BREAK",5,0);
//       writeMessage("THROTTLE",5,1);
//       break;
//     case SYSTEMOK:
//       break;
//   }
// }
