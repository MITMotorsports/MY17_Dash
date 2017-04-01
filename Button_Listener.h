#ifndef BUTTON_LISTENER_H
#define BUTTON_LISTENER_H

#include <Bounce2.h>
#include <Timer.h>

#include "Can_Controller.h"
#include "Led_Controller.h"
#include "Can_Ids.h"
#include "Pins.h"

class Button_Listener {
    public:
        static void begin();
        static void listen();
        static void sendEnableRequest();
        static void sendDisableRequest();
        static void pressRtdButton();
        static void releaseRtdButton();
};

#endif
