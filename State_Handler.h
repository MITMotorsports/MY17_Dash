#ifndef STATE_HANDLER_H
#define STATE_HANDLER_H

#include <Debouncer.h>
#include <PciManager.h>

#include <MY17_Can_Library.h>

#include "Led_Controller.h"

//handles the following things
//IMD fault
//AMS fault
//RTD signal
//HV enabled

class State_Handler {
  public:
    static void handle_Vcu_DashHeartbeat(Can_Vcu_DashHeartbeat_T *msg);
  private:
    static void setLightHelper(LightType type, bool desiredState);
};
#endif // STATE_HANDLER_H
