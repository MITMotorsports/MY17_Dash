#ifndef STATE_HANDLER_H
#define STATE_HANDLER_H

#include <Debouncer.h>
#include <PciManager.h>
#include "Led_Controller.h"
#include "Can_Controller.h"

//handles theh following things
//IMD fault
//AMS fault
//RTD signal
//HV enabled

class State_Handler {
    public:
    	static void begin();
    	static void handleMessage(Frame& frame);
    private:
	    static void processVcuMessage(Frame& message);
        static void setLightHelper(LightType type, uint8_t desiredState);
};
#endif // STATE_HANDLER_H
