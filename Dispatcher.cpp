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
  switch(type) {
    case Can_No_Msg:
      break;
    case Can_Error_Msg:
      // TODO error handling
      break;
    case Can_Unknown_Msg:
      Frame f;
      Can_Unknown_Read(&f);
      break;
    default:
      Page_Manager::process_msg(type);
      break;
  }
}

void Dispatcher::processButtonInputs() {
  Button_Listener::listen();
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
