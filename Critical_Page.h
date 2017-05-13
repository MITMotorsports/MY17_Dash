#ifndef CRITICAL_PAGE_H
#define CRITICAL_PAGE_H

#include <MY17_Can_Library.h>

#include "Page.h"

class Critical_Page : public Page {
  public:
    void top(String& line);
    void bottom(String& line);
    void process_Vcu_DashHeartbeat(Can_Vcu_DashHeartbeat_T *msg);
    void process_CurrentSensor_Power(Can_CurrentSensor_Power_T *msg);
    void process_Bms_Heartbeat(Can_Bms_Heartbeat_T *msg);
    void process_Bms_CellTemps(Can_Bms_CellTemps_T *msg);
    void process_Bms_PackStatus(Can_Bms_PackStatus_T *msg);
  private:
    void displayLowestCell(String& line);
    void displaySoc(String& line);
    void displayTemp(String& line);
    void displayPower(String& line);
};

#endif
