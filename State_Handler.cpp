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
  uint8_t RTD = message.body[0];
  uint8_t AMS = message.body[1];
  uint8_t IMD = message.body[2];
  uint8_t HV = message.body[3];

  if (RTD == 0) {
    Led_Controller::setRTDLight(LightState::OFF);
}
  else if(RTD == 1){
    Led_Controller::setRTDLight(LightState::ON);
}
  else {
    Serial.println("Invalid State");
}
  if (AMS == 0){
    Led_Controller::setAMSLight(LightState::OFF);
}
  else if (AMS == 1){

    Led_Controller::setAMSLight(LightState::ON);
}
  else {
    Serial.println("Invalid State");
}
  if (IMD == 0){
    Led_Controller::setIMDLight(LightState::OFF);
}
  else if (IMD == 1) {

    Led_Controller::setIMDLight(LightState::ON);
}
  else{
    Serial.println("Invalid State");
}
  if (HV == 0) {

    Led_Controller::setRTDLight(LightState::OFF);
}
  else if (HV == 1) {

    Led_Controller::setRTDLight(LightState::ON);
}
  else {
      Serial.println("Invalid State");
  }
}
