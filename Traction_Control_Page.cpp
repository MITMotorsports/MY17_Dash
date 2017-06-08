#include "Traction_Control_Page.h"

bool tc_enabled = false;

int16_t raw_torque = 0;
int16_t controlled_torque = 0;

void num_to_five_char_string(int16_t num, String& output);

void Traction_Control_Page::open() {
  // TODO
}

void Traction_Control_Page::screen(String& top, String& bottom) {
  top.concat("TC?");
  bottom.concat(tc_enabled ? "ENA" : "DIS");

  top.concat(" REG_TQ ");
  num_to_five_char_string(raw_torque, top);

  bottom.concat(" CTR_TQ ");
  num_to_five_char_string(controlled_torque, bottom);
}

void Traction_Control_Page::close() {
  // TODO
}

void Traction_Control_Page::process_Vcu_DashHeartbeat(Can_Vcu_DashHeartbeat_T *msg) {
  tc_enabled = msg->traction_control;
}

void Traction_Control_Page::process_FrontCanNode_DriverOutput(Can_FrontCanNode_DriverOutput_T *msg) {
  raw_torque = msg->torque_before_control;
  controlled_torque = msg->torque;
}

void num_to_five_char_string(int16_t num, String& output) {
  if (num < 0) {
    num = 0;
  }

  String num_string = String(num);
  uint8_t len = num_string.length();
  for (int i = 0; i < 5 - len; i++) {
    output.concat(" ");
  }
  for (int i = 0; i < len; i++) {
    output.concat(num_string.charAt(i));
  }
}
