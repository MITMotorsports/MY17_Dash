#include "Current_Sensor_Handler.h"

#include <MY17_Can_Library.h>

#include "Critical_Page.h"
#include "Pins.h"

void Current_Sensor_Handler::begin() {
    ///what do we need to initialize??
}

void Current_Sensor_Handler::handle_Power() {
  Can_CurrentSensor_Power_T msg;
  Can_CurrentSensor_Power_Read(&msg);

  int32_t power_dW = msg.power_W;
  power_dW = max(power_dW, 0); //can only display positive values on dash
  //cast to double and convert to KW
  double power_dkW = power_dW/1000.0;
  Critical_Page::setNumeric(TEMP, power_dkW);
}
