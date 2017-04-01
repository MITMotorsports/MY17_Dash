#include "VCU_Handler.h"
#include "Critical_Page.h"
#include "Led_Controller.h"
#include "Lcd_Controller.h"
#include "Can_Ids.h"
#include "Pins.h"

void Vcu_Handler::begin() {
    ///what do we need to initialize??
}

void Vcu_Handler::handleMessage(Frame& frame) {
    Serial.print("Received message with id ");
    Serial.println(frame.id);
  switch(frame.id) {
    case VCU_LED_ID:
      processVcuLedMessage(frame);
      break;
    case VCU_LIMP_ID:
      processVcuLimpMessage(frame);
      break;
    case VCU_AERO_ID:
      processVcuAeroMessage(frame);
      break;
    case VCU_TC_ID:
      processVcuTcMessage(frame);
      break;
    case VCU_REGEN_ID:
      processVcuRegenMessage(frame);
      break;
    case VCU_LV_ID:
      processVcuLvMessage(frame);
      break;
    }
}

void Vcu_Handler::processVcuLedMessage(Frame& message) {
  uint8_t rtd_msg = message.body[0];
  uint8_t ams_msg = message.body[1];
  uint8_t imd_msg = message.body[2];
  uint8_t hv_msg = message.body[3];
  setLightHelper(RTD, rtd_msg);
  setLightHelper(AMS, ams_msg);
  setLightHelper(IMD, imd_msg);
  setLightHelper(HV, hv_msg);
}

void Vcu_Handler::setLightHelper(LightType type, uint8_t desiredState){
  if (desiredState == 1){
      Led_Controller::setLight(type, LEDON);
  }
  else if(desiredState == 2){
      Led_Controller::setLight(type, LEDOFF);
  }
  else{
      // Led_Controller::setLight(type, LEDOFF);
      Serial.println("No change in state");
  }
}

void Vcu_Handler::processVcuLimpMessage(Frame& message){
  bool state = true;//stuff
  Critical_Page::setFlag(LIMP, state);
}

void Vcu_Handler::processVcuAeroMessage(Frame& message){
  bool state = true;//stuff
  Critical_Page::setFlag(AERO, state);
}

void Vcu_Handler::processVcuTcMessage(Frame& message){
  bool state = true;//stuff
  Critical_Page::setFlag(TC, state);
}

void Vcu_Handler::processVcuRegenMessage(Frame& message){
  bool state = true;//stuff
  Critical_Page::setFlag(REGEN, state);
}

void Vcu_Handler::processVcuLvMessage(Frame& message){
  bool state = true;//stuff
  Critical_Page::setFlag(LV, state);
}
