#ifndef _PAGE_H_
#define _PAGE_H_

#include <Arduino.h>

#include <MY17_Can_Library.h>

class Page {
  public:
    virtual void begin();
    virtual bool shouldDisplay();
    virtual void screen(String& top, String& bottom);

    virtual void process_FrontCanNode_DriverOutput(Can_FrontCanNode_DriverOutput_T *msg);
    virtual void process_FrontCanNode_RawValues(Can_FrontCanNode_RawValues_T *msg);
    virtual void process_FrontCanNode_WheelSpeed(Can_FrontCanNode_WheelSpeed_T *msg);
    virtual void process_RearCanNode_WheelSpeed(Can_RearCanNode_WheelSpeed_T *msg);

    virtual void process_Vcu_BmsHeartbeat(Can_Vcu_BmsHeartbeat_T *msg);
    virtual void process_Vcu_DashHeartbeat(Can_Vcu_DashHeartbeat_T *msg);

    virtual void process_Bms_Heartbeat(Can_Bms_Heartbeat_T *msg);
    virtual void process_Bms_CellTemps(Can_Bms_CellTemps_T *msg);
    virtual void process_Bms_PackStatus(Can_Bms_PackStatus_T *msg);
    virtual void process_Bms_Error(Can_Bms_Error_T *msg);

    virtual void process_MC_DataReading(Can_MC_DataReading_T *msg);
    // TODO
    // virtual void process_MC_ErrorAndWarning(Can_MC_ErrorAndWarning_T *msg);
    // virtual void process_MC_State(Can_MC_State_T *msg);

    virtual void process_CurrentSensor_Power(Can_CurrentSensor_Power_T *msg);
    virtual void process_CurrentSensor_Voltage(Can_CurrentSensor_Voltage_T *msg);
    virtual void process_CurrentSensor_Current(Can_CurrentSensor_Current_T *msg);
    virtual void process_CurrentSensor_Energy(Can_CurrentSensor_Energy_T *msg);
};
#endif // _PAGE_H_
