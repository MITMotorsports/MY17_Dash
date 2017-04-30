#include "Bms_Handler.h"

#include <MY17_Can_Library.h>

#include "Critical_Page.h"
#include "Pins.h"
void Bms_Handler::begin() {
    ///what do we need to initialize??
}

void Bms_Handler::handle_CellTemps() {
  Can_Bms_CellTemps_T msg;
  Can_Bms_CellTemps_Read(&msg);
  Critical_Page::process_Bms_CellTemps(&msg);
}

void Bms_Handler::handle_Heartbeat() {
  Can_Bms_Heartbeat_T msg;
  Can_Bms_Heartbeat_Read(&msg);
  Critical_Page::process_Bms_Heartbeat(&msg);
}
