#ifndef _WHEEL_SPEED_PAGE_H_
#define _WHEEL_SPEED_PAGE_H_

#include <MY17_Can_Library.h>

#include "Page.h"

class Wheel_Speed_Page : public Page {
  public:
    void screen(String& top, String& bottom);
    void process_FrontCanNode_WheelSpeed(Can_FrontCanNode_WheelSpeed_T *msg);
    void process_RearCanNode_WheelSpeed(Can_RearCanNode_WheelSpeed_T *msg);
};

#endif // _WHEEL_SPEED_PAGE_H_
