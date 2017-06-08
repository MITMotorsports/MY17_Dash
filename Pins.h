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

//PA5, AD3, 75
const int RTD_BUTTON_PIN = 25;

//PD2, INT2, RXD1, physical_pin_45
const int DASH_LEFT_BUTTON_PIN = 19;

//PD3, INT3, TXD1, physical_pin_46
const int DASH_RIGHT_BUTTON_PIN = 18;

//PD6, T1, physical_pin_49
const int STEERING_LEFT_BUTTON_PIN = PD6;

//PD7, T0, physical_pin_50
const int STEERING_RIGHT_BUTTON_PIN = 38;

//PF7, 90
const int BUZZER_PIN = 61;

//LCD Pins
//PK0, PCINT16, ADC8, A8
const int DB7_LCD_PIN = 62;

//PK1, PCINT17, ADC9, A9
const int DB6_LCD_PIN = 63;

//PK2, PCINT18, ADC10, A10
const int DB5_LCD_PIN = 64;

//PK3, PCINT19, ADC11, A11
const int DB4_LCD_PIN = 65;

//PK4, PCINT20, ADC12, A12
const int E_LCD_PIN = 66;

//PK5, PCINT21, ADC13, A13
const int RS_LCD_PIN = 67;
#endif // PINS_H
