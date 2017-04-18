#include "Led_Controller.h"
#include "Pins.h"

void Led_Controller::begin(){
    pinMode(HV_LED_PIN, OUTPUT);
    pinMode(RTD_LED_PIN, OUTPUT);
    pinMode(IMD_LED_PIN, OUTPUT);
    pinMode(AMS_LED_PIN, OUTPUT);

    //TODO change initialiation to LOW to turn off lights
    digitalWrite(HV_LED_PIN, HIGH);
    digitalWrite(RTD_LED_PIN, HIGH);
    digitalWrite(IMD_LED_PIN, HIGH);
    digitalWrite(AMS_LED_PIN, HIGH);
}

void Led_Controller::setLightHardware(uint8_t lightPin, LightState state){
    digitalWrite(lightPin, state == ON ? LOW : HIGH);
}
// TODO change the logic of pulling pins high/low to turn on LED after new board comes back
void Led_Controller::setLight(LightType type, LightState state) {
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
