#include "Dispatcher.h"

//Magic timing library stuff
#include <SoftTimer.h>
#include <DelayRun.h>
#include <Debouncer.h>
#include <Bounce2.h>
#include <Timer.h>

#include <MY17_Can_Library.h>

#include "Led_Controller.h"
#include "State_Handler.h"
#include "Button_Listener.h"

static bool begun;

void dispatchPointer(Task*);
Task dispatchTask(0, dispatchPointer);

void Dispatcher::begin() {
  // Idempotent
  if(begun) {
    return;
  }
  begun = true;

  // Start serial bus
  Serial.begin(115200);

  // Initialize LED controller
  Led_Controller::begin();

  // Initialize CAN controller
  Can_Init(500000);

  // Initialize button listener
  Button_Listener::begin();

  Serial.println("Dash Initialized");
  SoftTimer.add(&dispatchTask);
}

void Dispatcher::processCanInputs() {
  Can_MsgID_T type = Can_MsgType();
  switch(type) {
    case Can_No_Msg:
      return;
    case Can_Vcu_DashHeartbeat_Msg:
      Can_Vcu_DashHeartbeat_T msg;
      Can_Vcu_DashHeartbeat_Read(&msg);
      State_Handler::handle_Vcu_DashHeartbeat(&msg);
    default:
      return;
  }
}

void Dispatcher::processButtonInputs() {
  Button_Listener::listen();
}

void Dispatcher::dispatch() {
  processCanInputs();
  processButtonInputs();
}

void dispatchPointer(Task*) {
  Dispatcher::dispatch();
}
