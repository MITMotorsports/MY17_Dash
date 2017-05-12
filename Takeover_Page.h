#ifndef _TAKEOVER_PAGE_H_
#define _TAKEOVER_PAGE_H_

#include <MY17_Can_Library.h>

#include "Page.h"

class Takeover_Page : public Page {
    bool shouldDisplay();
    void top(String& line);
    void bottom(String& line);
    void process_Vcu_DashHeartbeat(Can_Vcu_DashHeartbeat_T *msg);
    void process_FrontCanNode_DriverOutput(Can_FrontCanNode_DriverOutput_T *msg);
    void process_MC_ErrorAndWarning(Can_MC_ErrorAndWarning_T *msg);
};

#endif // _TAKEOVER_PAGE_H_
