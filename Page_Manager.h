#ifndef _PAGE_MANAGER_H_
#define _PAGE_MANAGER_H_

#include <MY17_Can_Library.h>

#include "Page.h"

enum {
  TAKEOVER_PAGE,
  CRITICAL_PAGE,
  NUM_PAGES
};

class Page_Manager {
  public:
    static void begin();
    static void display();
    static void process_msg(Can_MsgID_T type);
};

#endif // _PAGE_MANAGER_H_
