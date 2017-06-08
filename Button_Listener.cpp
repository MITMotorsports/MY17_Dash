#include "Button_Listener.h"

#include <Bounce2.h>
#include <MY17_Can_Library.h>

#define HOLD_TIMEOUT 500

Bounce bouncers[BUTTONS_LENGTH];
uint32_t touch_times[BUTTONS_LENGTH];

bool action_flags[BUTTONS_LENGTH * ACTIONS_LENGTH];

Button_Action_T NO_ACTION = {.button=BUTTONS_LENGTH, .action=ACTIONS_LENGTH};

void Button_Listener::begin() {
  pinMode(RTD_BUTTON_PIN, INPUT);
  pinMode(DASH_LEFT_BUTTON_PIN, INPUT);
  pinMode(DASH_RIGHT_BUTTON_PIN, INPUT);
  pinMode(STEERING_RIGHT_BUTTON_PIN, INPUT);

  bouncers[RTD_BUTTON].attach(RTD_BUTTON_PIN);
  bouncers[DASH_LEFT_BUTTON].attach(DASH_LEFT_BUTTON_PIN);
  bouncers[DASH_RIGHT_BUTTON].attach(DASH_RIGHT_BUTTON_PIN);
  bouncers[STEERING_RIGHT_BUTTON].attach(STEERING_RIGHT_BUTTON_PIN);

  // TODO when we figure out what pin this is
  // pinMode(STEERING_LEFT_BUTTON_PIN, INPUT);
  // bouncers[STEERING_LEFT_BUTTON].attach(STEERING_LEFT_BUTTON_PIN);

  for (int i = 0; i < BUTTONS_LENGTH; i++) {
    bouncers[i].interval(50);
  }
}

Button_Action_T Button_Listener::update() {
  // First, update button and action states
  for (int i = 0; i < BUTTONS_LENGTH; i++) {
    Button_T button = (Button_T)i;
    check(button);
  }

  // Now return appropriate action
  for (int j = 0; j < BUTTONS_LENGTH * ACTIONS_LENGTH; j++) {
    if (action_flags[j]) {
      action_flags[j] = false;
      Button_T button = (Button_T)(j * 4);
      Action_T action = (Action_T)(j % 4);
      return to_Button_Action(button, action);
    }
  }
  return NO_ACTION;
}

void Button_Listener::check(Button_T button) {
  uint32_t msTicks = millis();
  uint32_t *touch_time = &touch_times[button];
  Bounce debouncer = bouncers[button];

  // Read button state
  debouncer.update();

  // Check for touch
  if (debouncer.fell()) {
    *touch_time = millis();
    press(button);
  }

  bool hold_not_fired = (*touch_time != 0);

  // Check for release and tap
  if (debouncer.rose()) {
    if (hold_not_fired) {
      *touch_time = 0;
      tap(button);
    }
    release(button);
  }

  // Check for hold
  bool hold_time_reached = *touch_time + HOLD_TIMEOUT < msTicks;
  if (hold_not_fired && hold_time_reached) {
    *touch_time = 0;
    hold(button);
  }
}

Button_Action_T Button_Listener::to_Button_Action(Button_T button, Action_T action) {
  Button_Action_T result;
  result.button = button;
  result.action = action;
  return result;
}

void Button_Listener::press(Button_T button){
  Serial.print("Button pressed ");
  Serial.println(String(button));

  action_flags[button + TOUCH] = true;
}

void Button_Listener::tap(Button_T button) {
  Serial.print("Button tapped ");
  Serial.println(String(button));

  action_flags[button + TAP] = true;
}

void Button_Listener::hold(Button_T button) {
  Serial.print("Button held ");
  Serial.println(String(button));

  action_flags[button + HOLD] = true;
}

void Button_Listener::release(Button_T button){
  Serial.print("Button released ");
  Serial.println(String(button));

  action_flags[button + RELEASE] = true;
}
