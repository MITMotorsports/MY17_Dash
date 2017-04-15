#include "Bms_Handler.h"
#include "Critical_Page.h"
#include "Can_Ids.h"
#include "Pins.h"
void Bms_Handler::begin() {
    ///what do we need to initialize??
}

void Bms_Handler::handleMessage(Frame& frame) {
    Serial.print("Received message with id ");
    Serial.println(frame.id);
  switch(frame.id) {
    case BMS_TEMP_ID:
      processBmsTempMessage(frame);
      break;
    case BMS_SOC_ID:
      processBmsSocMessage(frame);
      break;
    }
}

void Bms_Handler::processBmsTempMessage(Frame& message) {
  //TODO handle the message
  int8_t value = 1;
  Critical_Page::setNumeric(TEMP, value);
}


void Bms_Handler::processBmsSocMessage(Frame& message){
  //TODO hanle the message
  int8_t value = 1;
  Critical_Page::setNumeric(SOC, value);
}
