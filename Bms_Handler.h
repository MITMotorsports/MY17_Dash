#ifndef BMS_HANDLER_h
#define BMS_HANDLER_h

#include "Can_Controller.h"


class Bms_Handler{
  public:
    static void begin ();
    static void handleMessage(Frame& frame);
  private:
    static void processBmsTempMessage(Frame& message);
    static void processBmsSocMessage(Frame& message);
};
#endif
