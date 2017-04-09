#ifndef BUTTON_LISTENER_H
#define BUTTON_LISTENER_H

#include <Bounce2.h>
#include <Timer.h>

#include "Can_Controller.h"
#include "Led_Controller.h"
#include "Can_Ids.h"
#include "Pins.h"

enum ButtonName  {
    RTD_BUTTON,
    LIMP_BUTTON,
    AERO_BUTTON
};

class Button_Listener {
    public:
        static void begin();
        static void listen();

        static void pressButton(ButtonName button);
        static void releaseButton(ButtonName button);

        static void sendEnableRequest();
        static void sendDisableRequest();
        static void sendLimpLongRequest();
        static void sendLimpShortRequest();
        static void sendAeroLongRequest();
        static void sendAeroShortRequest();

};

#endif
