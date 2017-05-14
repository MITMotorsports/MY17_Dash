#include "Takeover_Page.h"

#include <Arduino.h>

typedef enum {
  Takeover_Master_Reset = 0,
  Takeover_Driver_Reset,

  Takeover_Bms_Fault,
  Takeover_Imd_Fault,
  Takeover_Bspd_Fault,
  Takeover_Esd_Fault,
  Takeover_Tsms_Fault,

  Takeover_Front_Can_Dead,
  Takeover_Rear_Can_Dead,
  Takeover_Bms_Dead,
  Takeover_MC_Dead,
  Takeover_Dash_Dead,
  Takeover_Current_Sensor_Dead,

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

Takeover_Order getTakeover();
void updateTakeoverField(bool state, Takeover_Order idx);
void error_to_string(Takeover_Order error, String& top, String& bottom);

// TODO this is pretty hacky
static bool enabled = false;

bool Takeover_Page::shouldDisplay() {
  return takeover_bitfield != 0UL;
}

void Takeover_Page::screen(String& top, String& bottom) {
  Takeover_Order error = getTakeover();
  if (error != Takeover_Length) {
    top.concat("********");
    bottom.concat("********");
    error_to_string(error, top, bottom);
  }
}

void error_to_string(Takeover_Order error, String& top, String& bot) {
  switch(error) {
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
      bot.concat("     BMS");
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
    case Takeover_Driver_Reset_Open:
      top.concat("   PRESS");
      bot.concat(" D RESET");
      break;
    case Takeover_Precharge_Running:
      top.concat("AWAITING");
      bot.concat("PRE CHRG");
      break;
    case Takeover_Brake_Released:
      top.concat("   PRESS");
      bot.concat("   BRAKE");
      break;
    case Takeover_Rtd_Off:
      top.concat("    HOLD");
      bot.concat("     RTD");
      break;
    case Takeover_MC_Fault:
      top.concat("MTR CONT");
      bot.concat("   FAULT");
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

  // TODO when rear can node finished
  // updateTakeoverField(msg->heartbeat_rear_can_node_dead, Takeover_Rear_Can_Dead);
  updateTakeoverField(false, Takeover_Rear_Can_Dead);

  updateTakeoverField(msg->heartbeat_front_can_node_dead, Takeover_Front_Can_Dead);
  updateTakeoverField(msg->heartbeat_bms_dead, Takeover_Bms_Dead);
  updateTakeoverField(msg->heartbeat_dash_dead, Takeover_Dash_Dead);

  // TODO when we request/respond from mc
  // updateTakeoverField(msg->heartbeat_mc_dead, Takeover_MC_Dead);
  updateTakeoverField(false, Takeover_MC_Dead);

  updateTakeoverField(
      msg->heartbeat_current_sensor_dead, Takeover_Current_Sensor_Dead);

  updateTakeoverField(msg->reset_latch_open, Takeover_Driver_Reset_Open);
  updateTakeoverField(msg->precharge_running, Takeover_Precharge_Running);

  enabled = msg->rtd_light;

  bool rtd_not_on = !enabled;
  Serial.println(String(rtd_not_on));

  updateTakeoverField(rtd_not_on, Takeover_Rtd_Off);

  updateTakeoverField(msg->master_reset_not_initialized, Takeover_Master_Reset);
  updateTakeoverField(msg->driver_reset_not_initialized, Takeover_Driver_Reset);
}

void Takeover_Page::process_FrontCanNode_DriverOutput(
    Can_FrontCanNode_DriverOutput_T *msg) {

  updateTakeoverField(msg->throttle_implausible, Takeover_Throttle_Implausible);
  updateTakeoverField(
      msg->brake_throttle_conflict, Takeover_Brake_Throttle_Conflict);

  bool brake_released = !msg->brake_engaged;
  updateTakeoverField(!enabled && brake_released, Takeover_Brake_Released);
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
