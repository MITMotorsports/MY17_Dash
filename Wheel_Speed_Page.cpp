#include "Wheel_Speed_Page.h"

uint32_t front_left_us = 0;
uint32_t front_right_us = 0;
uint32_t rear_left_us = 0;
uint32_t rear_right_us = 0;

#define CLICKS_PER_REV 23UL
#define MICROSECONDS_PER_SECOND 1000000UL
#define SECONDS_PER_MINUTE 60UL

uint32_t click_time_to_rpm(uint32_t click_time);
void num_to_four_char_string(uint32_t num, String& output);

void Wheel_Speed_Page::screen(String& top, String& bottom) {
  num_to_four_char_string(click_time_to_rpm(front_left_us), top);
  top.concat("RPM ");
  num_to_four_char_string(click_time_to_rpm(front_right_us), top);
  top.concat("RPM ");

  num_to_four_char_string(click_time_to_rpm(rear_left_us), bottom);
  bottom.concat("RPM ");
  num_to_four_char_string(click_time_to_rpm(rear_right_us), bottom);
  bottom.concat("RPM ");
}

void Wheel_Speed_Page::process_FrontCanNode_WheelSpeed(Can_FrontCanNode_WheelSpeed_T *msg) {
  front_left_us = msg->front_left_wheel_speed;
  front_right_us = msg->front_right_wheel_speed;
}

void Wheel_Speed_Page::process_RearCanNode_WheelSpeed(Can_RearCanNode_WheelSpeed_T *msg) {
  rear_left_us = msg->rear_left_wheel_speed;
  rear_right_us = msg->rear_right_wheel_speed;
}

uint32_t click_time_to_rpm(uint32_t us_per_click) {
  if (us_per_click == 0) {
    return 0;
  }

  const float us_per_rev = (us_per_click * CLICKS_PER_REV) * 1.0;
  const float s_per_rev = us_per_rev / MICROSECONDS_PER_SECOND;

  const uint32_t rev_per_s = (uint32_t)(1.0 / s_per_rev);
  const uint32_t rev_per_min = rev_per_s / SECONDS_PER_MINUTE;
  return rev_per_min;
}

void num_to_four_char_string(uint32_t num, String& output) {
  String num_string = String(num);
  uint8_t len = num_string.length();
  for (int i = 0; i < 4 - len; i++) {
    output.concat(" ");
  }
  for (int i = 0; i < len; i++) {
    output.concat(num_string.charAt(i));
  }
}
