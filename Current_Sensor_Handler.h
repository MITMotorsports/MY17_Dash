#ifndef CURRENT_SENSOR_HANDLER_h
#define CURRENT_SENSOR_HANDLER_h

#include <MY17_Can_Library.h>

class Current_Sensor_Handler{
  public:
    static void begin ();
    static void handle_Power();
};

#endif
