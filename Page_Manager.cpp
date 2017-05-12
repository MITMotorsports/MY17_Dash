#include "Page_Manager.h"

#include "Buzzer.h"
#include "Led_Controller.h"
#include "Lcd_Controller.h"

#include "Page.h"
#include "Critical_Page.h"
#include "Takeover_Page.h"
//#include "Misc_Page.h"

static Page pages[NUM_PAGES];
static uint8_t curr_page = TAKEOVER_PAGE;

static String last_top = "                ";
static String last_bottom = "                ";

void Page_Manager::begin() {
  pages[TAKEOVER_PAGE] = Takeover_Page();
  pages[CRITICAL_PAGE] = Critical_Page();
}

void Page_Manager::display() {
  String top = "";
  String bottom = "";

  // Takeover page only displays if it's selected and is a takeover
  if (curr_page == TAKEOVER_PAGE && pages[TAKEOVER_PAGE].shouldDisplay()) {
    set_display(pages[TAKEOVER_PAGE], top, bottom);
  }

  // Critical page displays if takeover is selected and no takeover active
  else if (curr_page == TAKEOVER_PAGE || curr_page == CRITICAL_PAGE){
    set_display(pages[CRITICAL_PAGE], top, bottom);
  }

  // TODO implement other pages
  else {
    top = "                ";
    bottom = "                ";
  }

  do_display(top, bottom);
}

void Page_Manager::do_display(String& top, String& bottom) {
  if (!top.equals(last_top)) {
    last_top = top;
    Lcd_Controller::writeMessage(last_top, 0, 0);
  }
  if (!bottom.equals(last_bottom)) {
    last_bottom = bottom;
    Lcd_Controller::writeMessage(last_bottom, 0, 1);
  }
}

void Page_Manager::set_display( Page& page, String& top, String& bottom) {
  page.top(top);
  page.bottom(bottom);
}

/**** process_msg is more complicated - see boilerplate section if curious ****/


/******************* Private Methods ********************/

void process_Vcu_DashHeartbeat();
void update_leds(Can_Vcu_DashHeartbeat_T *msg);
void update_buzzer(Can_Vcu_DashHeartbeat_T *msg);

void process_Vcu_DashHeartbeat() {
  Can_Vcu_DashHeartbeat_T msg;
  Can_Vcu_DashHeartbeat_Read(&msg);

  update_leds(&msg);
  update_buzzer(&msg);
  for (int i = 0; i < NUM_PAGES; i++) {
    pages[i].process_Vcu_DashHeartbeat(&msg);
  }
}

void update_leds(Can_Vcu_DashHeartbeat_T *msg) {
  Led_Controller::setLight(RTD, msg->rtd_light ? LED_ON : LED_OFF);
  Led_Controller::setLight(AMS, msg->ams_light ? LED_ON : LED_OFF);
  Led_Controller::setLight(IMD, msg->imd_light ? LED_ON : LED_OFF);
  Led_Controller::setLight(HV, msg->hv_light ? LED_ON : LED_OFF);
}

static bool enabled = false;
void update_buzzer(Can_Vcu_DashHeartbeat_T *msg) {
  if (msg->rtd_light && !enabled) {
    Buzzer::trigger(1500);
  }
  enabled = msg->rtd_light;
}

/******************************************************************************

    Boilerplate code below (should not need to modify unless CAN library changes).

    HACK ALERT!!!!!
    Becuse of the backwards compatibility with C requirement for the CAN library,
    we cannot use templates to implement repetitive logic for typed parameters.
    Instead we either have to rewrite the same boilerplate logic for every
    single CAN message type, or we have to use evil unreadable and hacky macros.
    I chose the latter because it's more ready for change, and I am lazy.

******************************************************************************/

#define PROCESS(name) \
  void process_ ## name() { \
    Can_ ## name ## _T msg; \
    Can_ ## name ## _Read(&msg); \
    for (int i = 0; i < NUM_PAGES; i++) { \
      pages[i].process_ ## name(&msg); \
    } \
  }

#define CASE(name) \
  case Can_ ## name ## _Msg: \
    process_ ## name(); \
    break; \

PROCESS(FrontCanNode_DriverOutput)
PROCESS(FrontCanNode_RawValues)
PROCESS(FrontCanNode_WheelSpeed)
PROCESS(RearCanNode_WheelSpeed)

PROCESS(Vcu_BmsHeartbeat)
// Dash heartbeat is special case because of LED/Buzzer, see above.

PROCESS(Bms_Heartbeat)
PROCESS(Bms_CellTemps)
PROCESS(Bms_PackStatus)
PROCESS(Bms_Error)

PROCESS(MC_DataReading)
// TODO
// PROCESS(MC_ErrorAndWarning)
// PROCESS(MC_State)

PROCESS(CurrentSensor_Power)
PROCESS(CurrentSensor_Voltage)
PROCESS(CurrentSensor_Current)
PROCESS(CurrentSensor_Energy)

void Page_Manager::process_msg(Can_MsgID_T type) {
  switch(type) {

    CASE(FrontCanNode_DriverOutput)
    CASE(FrontCanNode_RawValues)
    CASE(FrontCanNode_WheelSpeed)
    CASE(RearCanNode_WheelSpeed)

    CASE(Vcu_BmsHeartbeat)
    // No special behavior for dash necessary here
    CASE(Vcu_DashHeartbeat)

    CASE(Bms_Heartbeat)
    CASE(Bms_CellTemps)
    CASE(Bms_PackStatus)
    CASE(Bms_Error)

    CASE(MC_DataReading)
    // TODO
    // CASE(MC_ErrorAndWarning)
    // CASE(MC_State)

    CASE(CurrentSensor_Power)
    CASE(CurrentSensor_Voltage)
    CASE(CurrentSensor_Current)
    CASE(CurrentSensor_Energy)

    case Can_No_Msg:
      break;
    case Can_Error_Msg:
      // TODO error handling
      break;
    case Can_Unknown_Msg:
      Frame f;
      Can_Unknown_Read(&f);
      break;
    default:
      // Do nothing
      break;
  }
}
