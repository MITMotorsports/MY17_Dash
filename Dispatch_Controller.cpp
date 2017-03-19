#include "Dispatch_Controller.h"

//Magic timing library stuff
#include <SoftTimer.h>
#include <DelayRun.h>
#include <Debouncer.h>
#include <Bounce2.h>

#include "Can_Controller.h"
#include "Led_Controller.h"
#include "State_Handler.h"
#include "Button_Listener.h"
Dispatch_Controller::Dispatch_Controller()
: begun(false)
{

}

// Slight hack: invoke static member accessor function in non-member function
// pointer to work with SoftTimer's bullshit.
// TODO: Tighten this loop if perf problems arise
void dispatchPointer(Task*) {
  Dispatcher().dispatch();
}
Task stepTask(0, dispatchPointer);

void Dispatch_Controller::begin() {
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
  CAN().begin();

  // Initialize state controller
  State_Handler::begin();

  // Initialize button listener
  Button_Listener::begin();

  Serial.println("Dash Initialized");
  Frame msg = {.id=0x69, .body={10}, .len=1};
  CAN().write(msg);
  SoftTimer.add(&stepTask);
}

// Must define instance prior to use
Dispatch_Controller* Dispatch_Controller::instance = NULL;

Dispatch_Controller& Dispatch_Controller::getInstance() {
  if (!instance) {
    instance = new Dispatch_Controller();
    instance->begin();
  }
  return *instance;
}

Dispatch_Controller& Dispatcher() {
  return Dispatch_Controller::getInstance();
}

void Dispatch_Controller::dispatch() {
  if(CAN().msgAvailable()) {
  Frame frame = CAN().read();
  State_Handler::handleMessage(frame);
  }
  Button_Listener::listen();
}
