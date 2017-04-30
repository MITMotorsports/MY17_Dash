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

#include "Bms_Handler.h"
#include "Vcu_Handler.h"
#include "Button_Listener.h"
#include "Current_Sensor_Handler.h"
#include "Critical_Page.h"

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

  // Initialize handlers
  Vcu_Handler::begin();
  Bms_Handler::begin();

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
    case Can_Vcu_DashHeartbeat_Msg:
      Vcu_Handler::handle_DashHeartbeat();
      break;
    case Can_Bms_Heartbeat_Msg:
      Bms_Handler::handle_Heartbeat();
      break;
    case Can_Bms_CellTemps_Msg:
      Bms_Handler::handle_CellTemps();
      break;
    case Can_CurrentSensor_Power_Msg:
      Current_Sensor_Handler::handle_Power();
      break;
    default:
      Frame f;
      Can_Unknown_Read(&f);
      return;
  }
}

void Dispatcher::processButtonInputs() {
  Button_Listener::listen();
}

void Dispatcher::displayPages(){
  Critical_Page::display();
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
