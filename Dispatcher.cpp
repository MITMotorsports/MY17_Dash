#include "Dispatcher.h"

//Magic timing library stuff
#include <SoftTimer.h>
#include <DelayRun.h>
#include <Debouncer.h>
#include <Bounce2.h>

#include <MY17_Can_Library.h>

#include "Lcd_Controller.h"
#include "Led_Controller.h"
#include "Buzzer.h"

#include "Page_Manager.h"
#include "Button_Listener.h"

static bool begun;

void dispatchPointer(Task*);
Task dispatchTask(0, dispatchPointer);

void displayPointer(Task*);
Task displayTask(50, displayPointer);

void sendHeartbeat(Task*);
Task heartbeatTask(1000, sendHeartbeat);

void Dispatcher::begin() {
  // Idempotent
  if(begun) {
    return;
  }
  begun = true;

  // Start serial bus
  Serial.begin(115200);

  // Initialize CAN controller
  Can_Init(500000);

  // Initialize buzzer
  Buzzer::begin();

  // Initialize button listener
  Button_Listener::begin();

  // Initialize LED controller
  Led_Controller::begin();

  // Initialize LCD screen
  Lcd_Controller::begin();

  // Initialize pages
  Page_Manager::begin();

  Serial.println("Dash Initialized");
  SoftTimer.add(&dispatchTask);
  SoftTimer.add(&heartbeatTask);
  SoftTimer.add(&displayTask);
}

void Dispatcher::processCanInputs() {
  Can_MsgID_T type = Can_MsgType();
  Page_Manager::process_msg(type);
}

void Dispatcher::processButtonInputs() {
  Button_Action_T button_action = Button_Listener::update();

  Button_T button = button_action.button;
  Action_T action = button_action.action;
  if (button == DASH_LEFT_BUTTON || button == DASH_RIGHT_BUTTON) {
    // Dash buttons are delegated to page manager
    Page_Manager::process_action(button_action);
  }

  Can_Dash_Request_T msg;
  msg.type = CAN_DASH_REQUEST_NO_REQUEST;

  if (button == RTD_BUTTON) {
    if (action == TAP) {
      Serial.println("DISABLE");
      msg.type = CAN_DASH_REQUEST_RTD_DISABLE;
    } else if (action == HOLD) {
      Serial.println("ENABLE");
      msg.type = CAN_DASH_REQUEST_RTD_ENABLE;
    }
  }

  if (button == STEERING_RIGHT_BUTTON) {
    if (action == TOUCH) {
      msg.type = CAN_DASH_REQUEST_ACTIVE_AERO_ENABLE;
    }
    if (action == RELEASE) {
      msg.type = CAN_DASH_REQUEST_ACTIVE_AERO_DISABLE;
    }
  }

  if (button == STEERING_LEFT_BUTTON) {
    if (action == TAP) {
      msg.type = CAN_DASH_REQUEST_DATA_FLAG;
    }
  }

  if (msg.type != CAN_DASH_REQUEST_NO_REQUEST) {
    Can_Dash_Request_Write(&msg);
  }
}

void Dispatcher::displayPages(){
  Page_Manager::display();
}

void Dispatcher::dispatch() {
  processCanInputs();
  processButtonInputs();
}

void dispatchPointer(Task*) {
  Dispatcher::dispatch();
}

void displayPointer(Task*) {
  Dispatcher::displayPages();
}

void sendHeartbeat(Task*) {
  Can_Dash_Heartbeat_T msg;
  msg.ok = true;
  Can_Dash_Heartbeat_Write(&msg);
}
