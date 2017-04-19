#ifndef VCU_HANDLER_h
#define VCU_HANDLER_h

#include <MY17_Can_Library.h>

#include "Led_Controller.h"

class Vcu_Handler{
  public:
    static void begin ();
    static void handle_DashHeartbeat();
  private:
    static void updateLeds(Can_Vcu_DashHeartbeat_T *msg);
    static void updatePageInfo(Can_Vcu_DashHeartbeat_T *msg);
    static void setLight(LightType type, bool desiredState);
};
#endif
