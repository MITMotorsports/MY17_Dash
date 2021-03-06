#include "Wheel_Speed_Page.h"

uint32_t front_left_mRPM = 0UL;
uint32_t front_right_mRPM = 0UL;
uint32_t rear_left_mRPM = 0UL;
uint32_t rear_right_mRPM = 0UL;

uint32_t mrpm_to_rpm(uint32_t mrpm);
void num_to_four_char_string(uint32_t num, String& output);

void Wheel_Speed_Page::screen(String& top, String& bottom) {
  num_to_four_char_string(mrpm_to_rpm(front_left_mRPM), top);
  top.concat("RPM ");
  num_to_four_char_string(mrpm_to_rpm(front_right_mRPM), top);
  top.concat("RPM ");

  num_to_four_char_string(mrpm_to_rpm(rear_left_mRPM), bottom);
  bottom.concat("RPM ");
  num_to_four_char_string(mrpm_to_rpm(rear_right_mRPM), bottom);
  bottom.concat("RPM ");
}

void Wheel_Speed_Page::process_FrontCanNode_WheelSpeed(Can_FrontCanNode_WheelSpeed_T *msg) {
  front_left_mRPM = msg->front_left_wheel_speed_mRPM;
  front_right_mRPM = msg->front_right_wheel_speed_mRPM;
}

void Wheel_Speed_Page::process_RearCanNode_WheelSpeed(Can_RearCanNode_WheelSpeed_T *msg) {
  rear_left_mRPM = msg->rear_left_wheel_speed_mRPM;
  rear_right_mRPM = msg->rear_right_wheel_speed_mRPM;
}

void num_to_four_char_string(uint32_t num, String& output) {
  String num_string = String(num);
  uint8_t len = num_string.length();
  if (len > 4) {
    output.concat("9999");
    return;
  }
  for (int i = 0; i < 4 - len; i++) {
    output.concat(" ");
  }
  for (int i = 0; i < len; i++) {
    output.concat(num_string.charAt(i));
  }
}

uint32_t mrpm_to_rpm(uint32_t mrpm) {
  return mrpm / 1000UL;
}
