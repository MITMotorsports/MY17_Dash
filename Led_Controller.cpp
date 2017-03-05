#include "Led_Controller.h"
#include "Pins.h"

void Led_Controller::begin(){
    pinMode(HV_LED_PIN, OUTPUT);
    pinMode(RTD_LED_PIN, OUTPUT);
    pinMode(IMD_LED_PIN, OUTPUT);
    pinMode(AMS_LED_PIN, OUTPUT);
}

void Led_Controller::setIMDLight(LightState state) {
    switch (state) {
        case LightState::ON:
            digitalWrite(IMD_LED_PIN, HIGH);
            break;
        case LightState::OFF:
            digitalWrite(IMD_LED_PIN, LOW);
            break;
        default:
            Serial.println("Invalid State");
    }
}

void Led_Controller::setAMSLight(LightState state) {
    switch (state) {
        case LightState::ON:
            digitalWrite(AMS_LED_PIN, HIGH);
            break;
        case LightState::OFF:
            digitalWrite(AMS_LED_PIN, LOW);
            break;
        default:
            Serial.println("Invalid State");
    }
}

void Led_Controller::setRTDLight(LightState state) {
    switch (state) {
        case LightState::ON:
            digitalWrite(RTD_LED_PIN, HIGH);

            break;
        case LightState::OFF:
            digitalWrite(RTD_LED_PIN, LOW);
            break;
        default:
            Serial.println("Invalid State");
    }
}

void Led_Controller::setHVLight(LightState state) {
    switch (state) {
        case LightState::ON:
            digitalWrite(HV_LED_PIN, HIGH);

            break;
        case LightState::OFF:
            digitalWrite(HV_LED_PIN, LOW);
            break;
        default:
            Serial.println("Invalid State");
    }
}
