#ifndef _PAGE_MANAGER_H_
#define _PAGE_MANAGER_H_

#include <MY17_Can_Library.h>

#include "Page.h"

enum {
  CRITICAL_PAGE,
  OTHER_PAGE,
  TAKEOVER_PAGE,
  NUM_PAGES
};

class Page_Manager {
  public:
    static void begin();
    static void display();
    static void process_msg(Can_MsgID_T type);
  private:
    static void set_display(Page& page, String& top, String& bottom);
    static void do_display(String& top, String& bottom);
};

#endif // _PAGE_MANAGER_H_
