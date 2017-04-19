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

  int16_t cell_temp_dC = msg.avg_cell_temp;
  cell_temp_dC = min(cell_temp_dC, 1270);
  cell_temp_dC = max(cell_temp_dC, -1280);

  // This cast is safe because of the guards above
  int8_t cell_temp_C = (int8_t) (cell_temp_dC / 10);

  Critical_Page::setNumeric(TEMP, cell_temp_C);
}

void Bms_Handler::handle_Heartbeat() {
  Can_Bms_Heartbeat_T msg;
  Can_Bms_Heartbeat_Read(&msg);

  uint16_t soc_tenth_percent = msg.soc;
  soc_tenth_percent = max(soc_tenth_percent, 990);

  // This cast is safe because of the guards above
  int8_t soc_percent = (int8_t) (soc_tenth_percent / 10);

  Critical_Page::setNumeric(SOC, soc_percent);
}
