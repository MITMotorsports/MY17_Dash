#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <Arduino.h>

enum LightState {
    OFF,
    ON
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
    static void setLight(LightType type, LightState state);
  private:
    static void setLightHardware(uint8_t lightPin, LightState state);
};

#endif // LED_CONTROLLER_H
