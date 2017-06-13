#include "Button_Listener.h"

#include <Bounce2.h>
#include <MY17_Can_Library.h>

#define HOLD_TIMEOUT 500

// This MUST be an array of pointers, so as to not clone the Bounce objects
// upon access, thereby removing all of their attached pins and settings.
Bounce *bouncers[BUTTONS_LENGTH];

static Bounce rtd_bouncer;
static Bounce dl_bouncer;
static Bounce dr_bouncer;
static Bounce sl_bouncer;
static Bounce sr_bouncer;

uint32_t touch_times[BUTTONS_LENGTH];

bool action_flags[BUTTONS_LENGTH * ACTIONS_LENGTH];

Button_Action_T NO_ACTION = {.button=BUTTONS_LENGTH, .action=ACTIONS_LENGTH};

void Button_Listener::begin() {
  bouncers[RTD_BUTTON] = &rtd_bouncer;
  bouncers[DASH_LEFT_BUTTON] = &dl_bouncer;
  bouncers[DASH_RIGHT_BUTTON] = &dr_bouncer;
  bouncers[STEERING_LEFT_BUTTON] = &sl_bouncer;
  bouncers[STEERING_RIGHT_BUTTON] = &sr_bouncer;

  pinMode(RTD_BUTTON_PIN, INPUT);
  pinMode(DASH_LEFT_BUTTON_PIN, INPUT);
  pinMode(DASH_RIGHT_BUTTON_PIN, INPUT);
  pinMode(STEERING_LEFT_BUTTON_PIN, INPUT);
  pinMode(STEERING_RIGHT_BUTTON_PIN, INPUT);

  bouncers[RTD_BUTTON]->attach(RTD_BUTTON_PIN);
  bouncers[DASH_LEFT_BUTTON]->attach(DASH_LEFT_BUTTON_PIN);
  bouncers[DASH_RIGHT_BUTTON]->attach(DASH_RIGHT_BUTTON_PIN);
  bouncers[STEERING_LEFT_BUTTON]->attach(STEERING_LEFT_BUTTON_PIN);
  bouncers[STEERING_RIGHT_BUTTON]->attach(STEERING_RIGHT_BUTTON_PIN);

  for (int i = 0; i < BUTTONS_LENGTH; i++) {
    bouncers[i]->interval(50);
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
      Button_T button = (Button_T)(j / ACTIONS_LENGTH);
      Action_T action = (Action_T)(j % ACTIONS_LENGTH);
      return to_Button_Action(button, action);
    }
  }
  return NO_ACTION;
}

void Button_Listener::check(Button_T button) {
  uint32_t msTicks = millis();
  uint32_t *touch_time = &touch_times[button];
  Bounce *debouncer = bouncers[button];

  uint8_t button_idx = button * ACTIONS_LENGTH;

  // Read button state
  debouncer->update();

  // Check for touch
  if (debouncer->fell()) {
    *touch_time = msTicks;
    touch(button_idx);
  }

  // Check for release and tap
  if (debouncer->rose()) {
    bool hold_not_fired = (*touch_time != 0);
    if (hold_not_fired) {
      *touch_time = 0;
      tap(button_idx);
    }
    release(button_idx);
  }

  // Check for hold
  bool hold_not_fired = (*touch_time != 0);
  bool hold_time_reached = *touch_time + HOLD_TIMEOUT < msTicks;
  if (hold_not_fired && hold_time_reached) {
    *touch_time = 0;
    hold(button_idx);
  }
}

Button_Action_T Button_Listener::to_Button_Action(Button_T button, Action_T action) {
  Button_Action_T result;
  result.button = button;
  result.action = action;
  return result;
}

void Button_Listener::touch(uint8_t button_idx){
  action_flags[button_idx + TOUCH] = true;
}

void Button_Listener::tap(uint8_t button_idx) {
  action_flags[button_idx + TAP] = true;
}

void Button_Listener::hold(uint8_t button_idx) {
  action_flags[button_idx + HOLD] = true;
}

void Button_Listener::release(uint8_t button_idx){
  action_flags[button_idx + RELEASE] = true;
}
