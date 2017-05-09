#ifndef CRITICAL_PAGE_H
#define CRITICAL_PAGE_H

#include <MY17_Can_Library.h>

#include "Lcd_Controller.h"
#include "Page.h"

class Critical_Page : public Page {
  public:
    void begin();
    void top(String& line);
    void bottom(String& line);
    void process_Vcu_DashHeartbeat(Can_Vcu_DashHeartbeat_T *msg);
    void process_CurrentSensor_Power(Can_CurrentSensor_Power_T *msg);
    void process_FrontCanNode_DriverOutput(Can_FrontCanNode_DriverOutput_T *msg);
    void process_Bms_Heartbeat(Can_Bms_Heartbeat_T *msg);
    void process_Bms_CellTemps(Can_Bms_CellTemps_T *msg);
  private:
    void displaySoc(String& line);
    void displayTemp(String& line);
    void displayPower(String& line);
};

#endif
