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
  Critical_Page::process_CurrentSensor_Power(&msg);
}
