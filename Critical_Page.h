#ifndef CRITICAL_PAGE_H
#define CRITICAL_PAGE_H

#include "Lcd_Controller.h"

#include <MY17_Can_Library.h>

class Critical_Page {
  public:
    static void begin();
    static void process_Vcu_DashHeartbeat(Can_Vcu_DashHeartbeat_T *msg);
    static void process_CurrentSensor_Power(Can_CurrentSensor_Power_T *msg);
    static void process_FrontCanNode_DriverOutput(Can_FrontCanNode_DriverOutput_T *msg);
    static void process_Bms_Heartbeat(Can_Bms_Heartbeat_T *msg);
    static void process_Bms_CellTemps(Can_Bms_CellTemps_T *msg);
    static void display();
  private:
    static void displayTopRow();
    static void displayBottomRow();
    static void displaySoc(String& line);
    static void displayTemp(String& line);
    static void displayPower(String& line);
};

#endif
