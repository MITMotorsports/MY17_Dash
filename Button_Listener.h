#ifndef BUTTON_LISTENER_H
#define BUTTON_LISTENER_H

#include <Bounce2.h>
#include <Timer.h>

#include "Led_Controller.h"
#include "Pins.h"

typedef enum {
    RTD_BUTTON = 0,
    DASH_LEFT_BUTTON,
    DASH_RIGHT_BUTTON,
    STEERING_LEFT_BUTTON,
    STEERING_RIGHT_BUTTON,
    BUTTONS_LENGTH
} Button_T;

typedef enum {
  TOUCH = 0,
  TAP,
  HOLD,
  RELEASE,
  ACTIONS_LENGTH
} Action_T;

typedef struct {
  Button_T button;
  Action_T action;
} Button_Action_T;

class Button_Listener {
    public:
        static void begin();
        static Button_Action_T update();

    private:
        static void check(Button_T button);
        static Button_Action_T to_Button_Action(Button_T button, Action_T action);

        static void touch(uint8_t button_idx);
        static void tap(uint8_t button_idx);
        static void release(uint8_t button_idx);
        static void hold(uint8_t button_idx);
};

#endif
