#ifndef _TRACTION_CONTROL_PAGE_H_
#define _TRACTION_CONTROL_PAGE_H_

#include <MY17_Can_Library.h>

#include "Page.h"

class Traction_Control_Page : public Page {
  public:
    void open();
    void screen(String& top, String& bottom);
    void act(Action_T action);
    void process_Vcu_DashHeartbeat(Can_Vcu_DashHeartbeat_T *msg);
    void process_FrontCanNode_DriverOutput(Can_FrontCanNode_DriverOutput_T *msg);
    void close();
};

#endif // _TRACTION_CONTROL_PAGE_H_
