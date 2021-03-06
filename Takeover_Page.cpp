#include "Takeover_Page.h"

#include <Arduino.h>

typedef enum {
  Takeover_All_Can_Dead = 0,
  Takeover_Front_Can_Dead,
  Takeover_Rear_Can_Dead,
  Takeover_Bms_Dead,
  Takeover_MC_Dead,
  Takeover_Dash_Dead,
  Takeover_Current_Sensor_Dead,

  Takeover_Master_Reset,
  Takeover_Driver_Reset,

  Takeover_Bms_Fault,
  Takeover_Imd_Fault,
  Takeover_Bspd_Fault,
  Takeover_Esd_Fault,
  Takeover_Tsms_Fault,

  Takeover_Driver_Reset_Open,
  Takeover_Precharge_Running,

  Takeover_Brake_Released,
  Takeover_Rtd_Off,

  // Rationale: motor faults are cleared on RTD, so only surface them afterward
  Takeover_MC_Fault,

  Takeover_Throttle_Implausible,
  Takeover_Brake_Throttle_Conflict,

  Takeover_Length
} Takeover_Order;

static uint32_t takeover_bitfield = 0;

static uint16_t lv_voltage = 0;
static uint16_t brake_raw = 0;

Takeover_Order getTakeover();
void updateTakeoverField(bool state, Takeover_Order idx);
void error_to_string(Takeover_Order error, String& top, String& bottom);

// TODO this is pretty hacky
static bool enabled = false;

bool Takeover_Page::shouldDisplay() {
  return takeover_bitfield != 0UL;
}

// HACKHACK because takeover page view is identical to critical when there is no
// error, we need to have the actions also be identical.
void Takeover_Page::act(Action_T action) {
  if (action == TOUCH) {
    Can_Dash_Request_T msg;
    msg.type = CAN_DASH_REQUEST_LIMP_MODE_ENABLE;
    Can_Dash_Request_Write(&msg);
  }
}

void Takeover_Page::set_heartbeat_fail(bool status) {
  updateTakeoverField(status, Takeover_All_Can_Dead);
}

void concat_3(uint16_t num, String& line) {
  uint16_t disp = num;
  if (disp > 999) {
    disp = 999;
  }
  String disp_string = String(disp);

  if (disp >= 100) {
    line.concat(disp_string.charAt(0));
    line.concat(disp_string.charAt(1));
    line.concat(disp_string.charAt(2));
  } else if (disp >= 10) {
    line.concat('0');
    line.concat(disp_string.charAt(0));
    line.concat(disp_string.charAt(1));
  } else {
    line.concat('0');
    line.concat('0');
    line.concat(disp_string.charAt(0));
  }
}

void Takeover_Page::screen(String& top, String& bottom) {
  Takeover_Order error = getTakeover();
  // if (error < Takeover_Length) {
  //   top.concat("********");
  //   bottom.concat("********");
  //   error_to_string(error, top, bottom);
  // } else if (error < Takeover_Length) {
  if (error < Takeover_Length) {
    top.concat("GLV*BRK*");

    concat_3(lv_voltage, bottom);
    bottom.concat("*");
    concat_3(brake_raw, bottom);
    bottom.concat("*");

    error_to_string(error, top, bottom);
  }
}

void error_to_string(Takeover_Order error, String& top, String& bot) {
  switch(error) {
    case Takeover_All_Can_Dead:
      top.concat("   CHECK");
      bot.concat(" CAN BUS");
    case Takeover_Front_Can_Dead:
      top.concat("   CHECK");
      bot.concat(" FR NODE");
      break;
    case Takeover_Rear_Can_Dead:
      top.concat("   CHECK");
      bot.concat(" RE NODE");
      break;
    case Takeover_Bms_Dead:
      top.concat("   CHECK");
      bot.concat(" BMS CAN");
      break;
    case Takeover_MC_Dead:
      top.concat("   CHECK");
      bot.concat("MTR CONT");
      break;
    case Takeover_Dash_Dead:
      // Pretty sure this will never happen
      top.concat("   CHECK");
      bot.concat("    DASH");
      break;
    case Takeover_Current_Sensor_Dead:
      top.concat("   CHECK");
      bot.concat("CURR SNS");
      break;
    case Takeover_Master_Reset:
      top.concat("   PRESS");
      bot.concat(" M RESET");
      break;
    case Takeover_Driver_Reset:
      top.concat("   PRESS");
      bot.concat(" D RESET");
      break;
    case Takeover_Bms_Fault:
      top.concat("   CHECK");
      bot.concat("     BMS");
      break;
    case Takeover_Imd_Fault:
      top.concat("   CHECK");
      bot.concat("     IMD");
      break;
    case Takeover_Bspd_Fault:
      top.concat("   CHECK");
      bot.concat("    BSPD");
      break;
    case Takeover_Esd_Fault:
      top.concat("   CHECK");
      bot.concat("SHUTDOWN");
      break;
    case Takeover_Tsms_Fault:
      top.concat("   CHECK");
      bot.concat("    TSMS");
      break;
    case Takeover_Driver_Reset_Open:
      top.concat("   PRESS");
      bot.concat(" D RESET");
      break;
    case Takeover_Precharge_Running:
      top.concat("AWAITING");
      bot.concat("PRE CHRG");
      break;
    case Takeover_MC_Fault:
      top.concat("MTR CONT");
      bot.concat("   FAULT");
      break;
    case Takeover_Brake_Released:
      top.concat("   PRESS");
      bot.concat("   BRAKE");
      break;
    case Takeover_Rtd_Off:
      top.concat("    HOLD");
      bot.concat("     RTD");
      break;
    case Takeover_Throttle_Implausible:
      top.concat("THROTTLE");
      bot.concat(" IMPLAUS");
      break;
    case Takeover_Brake_Throttle_Conflict:
      top.concat(" RELEASE");
      bot.concat("THROTTLE");
      break;
    case Takeover_Length:
      // Should never happen
      top.concat("BUG!BUG!");
      bot.concat("BUG!BUG!");
      break;
  }
}

void Takeover_Page::process_Vcu_DashHeartbeat(Can_Vcu_DashHeartbeat_T *msg) {
  updateTakeoverField(msg->shutdown_bms, Takeover_Bms_Fault);
  updateTakeoverField(msg->shutdown_imd, Takeover_Imd_Fault);
  updateTakeoverField(msg->shutdown_bspd, Takeover_Bspd_Fault);
  updateTakeoverField(msg->shutdown_esd_drain, Takeover_Esd_Fault);
  updateTakeoverField(msg->tsms_off, Takeover_Tsms_Fault);

  updateTakeoverField(msg->heartbeat_front_can_node_dead, Takeover_Front_Can_Dead);

  // TODO shitcode
  updateTakeoverField(false, Takeover_Rear_Can_Dead);

  updateTakeoverField(msg->heartbeat_bms_dead, Takeover_Bms_Dead);
  updateTakeoverField(msg->heartbeat_dash_dead, Takeover_Dash_Dead);

  // TODO shitcode
  //updateTakeoverField(msg->heartbeat_mc_dead, Takeover_MC_Dead);
  updateTakeoverField(false, Takeover_MC_Dead);

  updateTakeoverField(
      msg->heartbeat_current_sensor_dead, Takeover_Current_Sensor_Dead);

  updateTakeoverField(msg->reset_latch_open, Takeover_Driver_Reset_Open);
  updateTakeoverField(msg->precharge_running, Takeover_Precharge_Running);

  enabled = msg->rtd_light;

  bool rtd_not_on = !enabled;

  updateTakeoverField(rtd_not_on, Takeover_Rtd_Off);

  updateTakeoverField(msg->master_reset_not_initialized, Takeover_Master_Reset);
  updateTakeoverField(msg->driver_reset_not_initialized, Takeover_Driver_Reset);

  lv_voltage = msg->lv_battery_voltage;
}

void Takeover_Page::process_FrontCanNode_DriverOutput(
    Can_FrontCanNode_DriverOutput_T *msg) {

  updateTakeoverField(msg->throttle_implausible, Takeover_Throttle_Implausible);
  updateTakeoverField(
      msg->brake_throttle_conflict, Takeover_Brake_Throttle_Conflict);

  bool brake_released = !msg->brake_engaged;
  updateTakeoverField(!enabled && brake_released, Takeover_Brake_Released);
}

void Takeover_Page::process_FrontCanNode_RawValues(Can_FrontCanNode_RawValues_T *msg) {
  brake_raw = msg->brake_1_raw;
}

void Takeover_Page::process_MC_ErrorAndWarning(Can_MC_ErrorAndWarning_T *msg) {
  // TODO once we add in mc fault requests
}

void updateTakeoverField(bool state, Takeover_Order idx) {
  if (state) {
    takeover_bitfield |= 1UL << idx;
  } else {
    takeover_bitfield &= ~(1UL << idx);
  }
}

Takeover_Order getTakeover() {
  for(uint8_t i = 0; i < Takeover_Length; i++) {
    if (takeover_bitfield & (1UL << i)) {
      // This cast is safe because Takeover_Order begins at zero and
      // is guaranteed by the for loop to be less than length, as long
      // as Takeover_Length is the final value in the enum
      return (Takeover_Order)i;
    }
  }
  return Takeover_Length;
}
