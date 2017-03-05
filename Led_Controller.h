#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include "LightState.h"

class Led_Controller {
  public:
    static void begin();
    static void setRTDLight(LightState state);
    static void setAMSLight(LightState state);
    static void setIMDLight(LightState state);
    static void setHVLight(LightState state);
};

#endif // LED_CONTROLLER_H
