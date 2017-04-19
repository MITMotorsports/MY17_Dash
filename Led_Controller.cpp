#include "Led_Controller.h"
#include "Pins.h"

void Led_Controller::begin(){
    pinMode(HV_LED_PIN, OUTPUT);
    pinMode(RTD_LED_PIN, OUTPUT);
    pinMode(IMD_LED_PIN, OUTPUT);
    pinMode(AMS_LED_PIN, OUTPUT);

    setLight(HV, LED_OFF);
    setLight(RTD, LED_OFF);
    setLight(IMD, LED_OFF);
    setLight(AMS, LED_OFF);
}

void Led_Controller::setLight(LightType type, LedLightState state) {
    switch (type) {
        case HV:
            setLightHardware(HV_LED_PIN, state);
            break;
        case RTD:
            setLightHardware(RTD_LED_PIN, state);
            break;
        case IMD:
            setLightHardware(IMD_LED_PIN, state);
            break;
        case AMS:
            setLightHardware(AMS_LED_PIN, state);
            break;
    }
}

void Led_Controller::setLightHardware(uint8_t lightPin, LedLightState state){
    digitalWrite(lightPin, state == LED_ON ? HIGH : LOW);
}
