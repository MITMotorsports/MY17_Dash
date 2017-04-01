#ifndef VCU_HANDLER_h
#define VCU_HANDLER_h

#include "Led_Controller.h"
#include "Can_Controller.h"


class Vcu_Handler{
  public:
    static void begin ();
    static void handleMessage(Frame& frame);
  private:
    static void processVcuLedMessage(Frame& message);
    static void processVcuLimpMessage(Frame& message);
    static void processVcuAeroMessage(Frame& message);
    static void processVcuTcMessage(Frame& message);
    static void processVcuRegenMessage(Frame& message);
    static void processVcuLvMessage(Frame& message);
    static void setLightHelper(LightType type, uint8_t desiredState);
};

#endif
