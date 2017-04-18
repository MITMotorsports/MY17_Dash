#include "State_Handler.h"

#include "Led_Controller.h"
#include "Pins.h"

void State_Handler::handle_Vcu_DashHeartbeat(Can_Vcu_DashHeartbeat_T *msg) {
  setLightHelper(RTD, msg->rtd_light);
  setLightHelper(AMS, msg->ams_light);
  setLightHelper(IMD, msg->imd_light);
  setLightHelper(HV, msg->hv_light);
}

void State_Handler::setLightHelper(LightType type, bool desiredState){
  Led_Controller::setLight(type, desiredState ? ON : OFF);
}
