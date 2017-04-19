#ifndef BMS_HANDLER_h
#define BMS_HANDLER_h

#include <MY17_Can_Library.h>

class Bms_Handler{
  public:
    static void begin ();
    static void handle_Heartbeat();
    static void handle_CellTemps();
};

#endif
