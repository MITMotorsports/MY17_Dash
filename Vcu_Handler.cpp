#include "Vcu_Handler.h"
#include "Critical_Page.h"
#include "Led_Controller.h"
#include "Lcd_Controller.h"
#include "Pins.h"
#include "Buzzer.h"

#include <MY17_Can_Library.h>

Buzzer buzzer;

void Vcu_Handler::begin() {
    ///what do we need to initialize??
}

void Vcu_Handler::handle_DashHeartbeat() {
  Can_Vcu_DashHeartbeat_T msg;
  Can_Vcu_DashHeartbeat_Read(&msg);
  Serial.print("Received message with id ");
  updateLeds(&msg);
  updatePageInfo(&msg);
}

void Vcu_Handler::updateLeds(Can_Vcu_DashHeartbeat_T *msg) {
  setLight(RTD, msg->rtd_light);
  setLight(AMS, msg->ams_light);
  setLight(IMD, msg->imd_light);
  setLight(HV, msg->hv_light);
}

void Vcu_Handler::updatePageInfo(Can_Vcu_DashHeartbeat_T *msg) {
  Critical_Page::setFlag(LIMP, msg->limp_mode);
  Critical_Page::setFlag(AERO, msg->active_aero);
  Critical_Page::setFlag(TC, msg->traction_control);
  Critical_Page::setFlag(REGEN, msg->regen);
  Critical_Page::setFlag(LV, msg->lv_warning);
  if (msg->rtd_light) {
    // TODO
    buzzer.trigger(100);
  }
}

void Vcu_Handler::setLight(LightType type, bool desiredState){
  Led_Controller::setLight(type, desiredState ? LED_ON : LED_OFF);
}
