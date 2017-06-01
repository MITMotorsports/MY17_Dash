#include "Critical_Page.h"
#include <Arduino.h>
#include <Wire.h>

#include <MY17_Can_Library.h>
#include <LiquidCrystal.h>

// From VCU
static bool limpOn = false;
static bool aeroOn = false;
static bool tcOn = false;
static bool regenOn = false;
static bool lvLow = false;

// From Current Sensor
static int16_t power_cW = 0;

// From BMS
static uint8_t soc = 0;
static uint8_t temp = 0;
static uint16_t min_cell_voltage = 0;
static bool dcdc_fault = false;

static String lastTop = "";
static String lastBottom = "";

void Critical_Page::process_Vcu_DashHeartbeat(Can_Vcu_DashHeartbeat_T *msg){
  limpOn = msg->limp_mode;
  aeroOn = msg->active_aero;
  tcOn = msg->traction_control;
  regenOn = msg->regen;
  lvLow = msg->lv_warning;
}

void Critical_Page::process_CurrentSensor_Power(Can_CurrentSensor_Power_T *msg)
{
  int32_t power = msg->power_W / 100;
  // This cast is safe because power will never go above 100 kW, or 1000 cW
  power_cW = (int16_t) power;
}

void Critical_Page::process_Bms_Heartbeat(Can_Bms_Heartbeat_T *msg) {
  soc = msg->soc;
  if (msg->dcdc_enable && msg->dcdc_fault) {
    // Fault line is on when trying to enable, no bueno
    dcdc_fault = true;
  } else {
    dcdc_fault = false;
  }
}

void Critical_Page::process_Bms_CellTemps(Can_Bms_CellTemps_T *msg) {
  // max_cell_temp is in deci-celsius
  int16_t max_temp_C = msg->max_cell_temp / 10;
  // Car temp should never go above 255C, so this cast is safe
  temp = (uint8_t) max_temp_C;
}

void Critical_Page::process_Bms_PackStatus(Can_Bms_PackStatus_T *msg) {
  // cell voltage is in mV
  min_cell_voltage = msg->min_cell_voltage;
}

void Critical_Page::screen(String& top, String& bottom) {
  top_line(top);
  bottom_line(bottom);
}

void Critical_Page::top_line(String& line) {
  line.concat(limpOn ? "LIMP" : "    ");
  line.concat(" ");
  line.concat(aeroOn ? "AE" : "  ");
  line.concat(" ");
  line.concat(tcOn ? "TC" : "  ");
  line.concat(" ");
  line.concat(regenOn ? "RE" : "  ");
  line.concat(" ");
  line.concat(dcdc_fault ? "LV" : "  ");
}

void Critical_Page::bottom_line(String& line) {
  // TODO when soc finished if ever
  //displaySoc(line);

  displayLowestCell(line);
  displayTemp(line);
  displayPower(line);
}

void Critical_Page::displayLowestCell(String& line) {
  if (min_cell_voltage < 100) {
    // Lowest cell is below 1 volt so something is very wrong
    line.concat("RIP:(");
  } else {
    String voltage_string = String(min_cell_voltage);
    line.concat(voltage_string.charAt(0));
    line.concat(".");
    line.concat(voltage_string.charAt(1));
    line.concat(voltage_string.charAt(2));
    line.concat("V");
    line.concat(" ");
  }
}
void Critical_Page::displaySoc(String& line) {
  if (soc > 99) {
    soc = 99;
  }
  line.concat("SOC");
  String soc_string = String(soc);
  bool two_digits = soc >= 10;
  // We have asserted above that SOC is 99 or below
  line.concat(two_digits ? soc_string.charAt(0) : '0');
  line.concat(two_digits ? soc_string.charAt(1) : soc_string.charAt(0));
  line.concat(" ");
}

void Critical_Page::displayTemp(String& line) {
  if (temp > 99) {
    temp = 99;
  }
  bool two_digits = temp >= 10;
  String temp_string = String(temp);
  line.concat(two_digits ? temp_string.charAt(0) : '0');
  line.concat(two_digits ? temp_string.charAt(1) : temp_string.charAt(0));
  line.concat("C");
}

void Critical_Page::displayPower(String& line) {
  int16_t power_disp = power_cW;
  if (power_cW < 0) {
    power_disp = -1 * power_disp;
    line.concat('-');
  } else {
    line.concat(' ');
  }
  if (power_cW > 999) {
    power_disp = 999;
  }

  String power_string = String(power_disp);
  if (power_disp >= 100) {
    line.concat(power_string.charAt(0));
    line.concat(power_string.charAt(1));
    line.concat('.');
    line.concat(power_string.charAt(2));
  } else if (power_disp >= 10) {
    line.concat('0');
    line.concat(power_string.charAt(0));
    line.concat('.');
    line.concat(power_string.charAt(1));
  } else {
    line.concat('0');
    line.concat('0');
    line.concat('.');
    line.concat(power_string.charAt(0));
  }
  line.concat("kW");
}
