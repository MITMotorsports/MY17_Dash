#ifndef DISPATCH_CONTROLLER_H
#define DISPATCH_CONTROLLER_H

#include "Can_Controller.h"

class Dispatch_Controller {
  public:
    static Dispatch_Controller& getInstance();
    void begin();
    void disable();
    void enable();
    void dispatch();
  private:
    Dispatch_Controller();
    void performEnableActions(Frame& frame);
    static Dispatch_Controller *instance;
    bool begun;
};

Dispatch_Controller& Dispatcher();

#endif // DISPATCH_CONTROLLER_H
