#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <Arduino.h>

enum LedLightState {
    LED_OFF,
    LED_ON
};

enum LightType{
    IMD,
    HV,
    RTD,
    AMS
};

class Led_Controller {
  public:
    static void begin();
    static void setLight(LightType type, LedLightState state);
  private:
    static void setLightHardware(uint8_t lightPin, LedLightState state);
};

#endif
