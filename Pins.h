#ifndef PINS_H
#define PINS_H

#include <Arduino.h>
#include <pins_arduino.h>


//LED
//PF0, 97, digital 54
const int HV_LED_PIN = 54;

//PF3, 94, digital 57
const int RTD_LED_PIN = 57;

//PF1, 96, digital 55
const int IMD_LED_PIN = 55;

//PF2, 95, digital 56
const int AMS_LED_PIN = 56;

//PD2, 45
const int SELECT_BUTTON_PIN = 19;

//PD3, 46
const int RIGHT_BUTTON_PIN = 18;

// TODO initialize PD4 - PD5 in a special way
//PD4, 47
const int DRIVER_RESET_BUTTON_PIN = PD4;

//PD5, 48
const int RTD_BUTTON_PIN = PD5;

//PD6, 49
const int STEERING_BUTTON_PIN = PD6;

//PD7, 50
const int LIMP_BUTTON_PIN = 38;

//PF7, 90
const int BUZZER_PIN = A7;

#endif // PINS_H
