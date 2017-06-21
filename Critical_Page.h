#ifndef CRITICAL_PAGE_H
#define CRITICAL_PAGE_H

#include <MY17_Can_Library.h>

#include "Page.h"

class Critical_Page : public Page {
  public:
    void screen(String& top, String& bottom);
    void act(Action_T action);
    void process_Vcu_DashHeartbeat(Can_Vcu_DashHeartbeat_T *msg);
    void process_CurrentSensor_Power(Can_CurrentSensor_Power_T *msg);
    void process_CurrentSensor_Voltage(Can_CurrentSensor_Voltage_T *msg);
    void process_Bms_Heartbeat(Can_Bms_Heartbeat_T *msg);
    void process_Bms_CellTemps(Can_Bms_CellTemps_T *msg);
    void process_Bms_PackStatus(Can_Bms_PackStatus_T *msg);
  private:
    void top_line(String& line);
    void bottom_line(String& line);
    void displayLowestCell(String& line);
    void displaySoc(String& line);
    void displayTemp(String& line);
    void displayPower(String& line);
    void num_to_three_char_string(int32_t num, String& output);
};

#endif
