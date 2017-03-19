#include "State_Handler.h"

#include "Led_Controller.h"
#include "Can_Ids.h"
#include "Pins.h"

void State_Handler::begin() {
    ///what do we need to initialize??
}

void State_Handler::handleMessage(Frame& frame) {
    Serial.print("Received message with id ");
    Serial.println(frame.id);
  switch(frame.id) {
    case VCU_ID:
      processVcuMessage(frame);
      break;
    }
}

void State_Handler::processVcuMessage(Frame& message) {
  uint8_t rtd_msg = message.body[0];
  uint8_t ams_msg = message.body[1];
  uint8_t imd_msg = message.body[2];
  uint8_t hv_msg = message.body[3];
  setLightHelper(RTD, rtd_msg);
  setLightHelper(AMS, ams_msg);
  setLightHelper(IMD, imd_msg);
  setLightHelper(HV, hv_msg);
}

void State_Handler::setLightHelper(LightType type, uint8_t desiredState){
  if (desiredState == 1){
      Led_Controller::setLight(type, ON);
  }
  else if(desiredState == 0){
      Led_Controller::setLight(type, OFF);
  }
  else{
      Led_Controller::setLight(type, OFF);
      Serial.println("Invalid state for light");
  }
}
