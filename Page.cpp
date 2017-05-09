#include "Page.h"

// All methods have default no-op implementations
void Page::begin() { }
bool Page::shouldDisplay() { return true; }
void Page::top(String& line) { }
void Page::bottom(String& line) { }

void Page::process_FrontCanNode_DriverOutput(Can_FrontCanNode_DriverOutput_T *msg) { }
void Page::process_FrontCanNode_RawValues(Can_FrontCanNode_RawValues_T *msg) { }
void Page::process_FrontCanNode_WheelSpeed(Can_FrontCanNode_WheelSpeed_T *msg) { }
void Page::process_RearCanNode_WheelSpeed(Can_RearCanNode_WheelSpeed_T *msg) { }

void Page::process_Vcu_BmsHeartbeat(Can_Vcu_BmsHeartbeat_T *msg) { }
void Page::process_Vcu_DashHeartbeat(Can_Vcu_DashHeartbeat_T *msg) { }

void Page::process_Bms_Heartbeat(Can_Bms_Heartbeat_T *msg) { }
void Page::process_Bms_CellTemps(Can_Bms_CellTemps_T *msg) { }
void Page::process_Bms_PackStatus(Can_Bms_PackStatus_T *msg) { }
void Page::process_Bms_Error(Can_Bms_Error_T *msg) { }

void Page::process_CurrentSensor_Power(Can_CurrentSensor_Power_T *msg) { }
void Page::process_CurrentSensor_Voltage(Can_CurrentSensor_Voltage_T *msg) { }
void Page::process_CurrentSensor_Current(Can_CurrentSensor_Current_T *msg) { }
void Page::process_CurrentSensor_Energy(Can_CurrentSensor_Energy_T *msg) { }
