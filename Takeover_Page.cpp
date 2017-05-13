#include "Takeover_Page.h"

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
  Takeover_Mc_Fault,

  Takeover_Throttle_Implausible,
  Takeover_Brake_Throttle_Conflict,

  Takeover_Length
} Takeover_Order;

static uint32_t takeover_bitfield = 0;

void topMsg(String& line, Takeover_Order idx);
void bottomMsg(String& line, Takeover_Order idx);
Takeover_Order getTakeover();
void updateTakeoverField(bool state, Takeover_Order idx);

// Shortcut to prevent looping twice
Takeover_Order lastError = Takeover_Length;

bool Takeover_Page::shouldDisplay() {
  return takeover_bitfield != 0;
}

void Takeover_Page::top(String& line) {
  lastError = getTakeover();
  if (lastError != Takeover_Length) {
    line.concat("********");
    topMsg(line, lastError);
  }
}

void Takeover_Page::bottom(String& line) {
  // TODO can remove this if somehow performance problems
  lastError = getTakeover();
  if (lastError != Takeover_Length) {
    line.concat("********");
    bottomMsg(line, lastError);
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
  updateTakeoverField(msg->heartbeat_mc_dead, Takeover_MC_Dead);
  updateTakeoverField(
      msg->heartbeat_current_sensor_dead, Takeover_Current_Sensor_Dead);

  updateTakeoverField(msg->reset_latch_open, Takeover_Driver_Reset_Open);
  updateTakeoverField(msg->precharge_running, Takeover_Precharge_Running);

  bool rtd_not_on = !msg->rtd_light;
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
  updateTakeoverField(brake_released, Takeover_Brake_Released);
}

void Takeover_Page::process_MC_ErrorAndWarning(Can_MC_ErrorAndWarning_T *msg) {
  // TODO once we add in mc fault requests
}

void updateTakeoverField(bool state, Takeover_Order idx) {
  if (state) {
    takeover_bitfield |= 1 << idx;
  } else {
    takeover_bitfield &= ~(1 << idx);
  }
}

Takeover_Order getTakeover() {
  for(uint8_t i = 0; i < Takeover_Length; i++) {
    if (takeover_bitfield & (1 << i)) {
      // This cast is safe because Takeover_Order begins at zero and
      // is guaranteed by the for loop to be less than length, as long
      // as Takeover_Length is the final value in the enum
      return (Takeover_Order)i;
    }
  }
  return Takeover_Length;
}

void topMsg(String& line, Takeover_Order idx) {
  switch(idx) {
    case Takeover_Bms_Fault:
    case Takeover_Imd_Fault:
    case Takeover_Bspd_Fault:
    case Takeover_Esd_Fault:
    case Takeover_Tsms_Fault:
    case Takeover_Front_Can_Dead:
    case Takeover_Rear_Can_Dead:
    case Takeover_Bms_Dead:
    case Takeover_MC_Dead:
    case Takeover_Dash_Dead:
    case Takeover_Current_Sensor_Dead:
      // Note: out of order but wanted to prevent duplicate code
    case Takeover_Mc_Fault:
      line.concat("   CHECK");
      break;

      // Note: out of order but wanted to prevent duplicate code
    case Takeover_Master_Reset:
    case Takeover_Driver_Reset:
    case Takeover_Driver_Reset_Open:
    case Takeover_Brake_Released:
      line.concat("   PRESS");
      break;

    case Takeover_Precharge_Running:
      line.concat("AWAITING");
      break;

    case Takeover_Rtd_Off:
      line.concat("    HOLD");
      break;

    case Takeover_Throttle_Implausible:
      line.concat("THROTTLE");
      break;

    case Takeover_Brake_Throttle_Conflict:
      line.concat(" RELEASE");
      break;

    case Takeover_Length:
      // Should never happen
      line.concat("BUG!BUG!");
      break;
  }
}

void bottomMsg(String& line, Takeover_Order idx) {
  switch(idx) {
    case Takeover_Master_Reset:
      line.concat(" M RESET");
      break;
    case Takeover_Bms_Fault:
    case Takeover_Bms_Dead:
      line.concat("     BMS");
      break;
    case Takeover_Imd_Fault:
      line.concat("     IMD");
      break;
    case Takeover_Bspd_Fault:
      line.concat("    BSPD");
      break;
    case Takeover_Esd_Fault:
      line.concat("SHUTDOWN");
      break;
    case Takeover_Driver_Reset_Open:
    case Takeover_Driver_Reset:
      line.concat(" D RESET");
      break;
    case Takeover_Tsms_Fault:
      line.concat("    TSMS");
      break;
    case Takeover_Front_Can_Dead:
      line.concat(" FR NODE");
      break;
    case Takeover_Rear_Can_Dead:
      line.concat(" RE NODE");
      break;
    case Takeover_MC_Dead:
    case Takeover_Mc_Fault:
      line.concat("MTR CONT");
      break;
    case Takeover_Dash_Dead:
      line.concat("    DASH");
      break;
    case Takeover_Current_Sensor_Dead:
      line.concat("CURR SNS");
      break;
    case Takeover_Precharge_Running:
      line.concat("PRE CHRG");
      break;
    case Takeover_Rtd_Off:
      line.concat(" RTD BTN");
      break;
    case Takeover_Throttle_Implausible:
      line.concat(" IMPLAUS");
      break;
    case Takeover_Brake_Released:
    case Takeover_Brake_Throttle_Conflict:
      line.concat("   BRAKE");
      break;
    case Takeover_Length:
      // Should never happen
      line.concat("BUG!BUG!");
      break;
  }
}

