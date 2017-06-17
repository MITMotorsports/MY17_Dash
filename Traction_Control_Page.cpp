#include "Traction_Control_Page.h"

bool tc_enabled = false;

int16_t raw_torque = 0;
int16_t controlled_torque = 0;

void num_to_five_char_string(int16_t num, String& output);

void Traction_Control_Page::open() {
  // TODO
}

void Traction_Control_Page::screen(String& top, String& bottom) {
  top.concat("TORQUE_REQ ");
  num_to_five_char_string(raw_torque, top);

  bottom.concat("TORQUE_FLT ");
  num_to_five_char_string(controlled_torque, bottom);
}

void Traction_Control_Page::act(Action_T action) {
  if (action == TAP) {
    Can_Dash_Request_T msg;
    msg.type = CAN_DASH_REQUEST_TRACTION_CONTROL_DISABLE;
    Can_Dash_Request_Write(&msg);
  }
  else if (action == HOLD) {
    Can_Dash_Request_T msg;
    msg.type = CAN_DASH_REQUEST_TRACTION_CONTROL_ENABLE;
    Can_Dash_Request_Write(&msg);
  }
}

void Traction_Control_Page::close() {
  // Disable traction control when exiting page
  Can_Dash_Request_T msg;
  msg.type = CAN_DASH_REQUEST_TRACTION_CONTROL_DISABLE;
  Can_Dash_Request_Write(&msg);
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
