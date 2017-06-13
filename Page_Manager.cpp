#include "Page_Manager.h"

#include <Arduino.h>

#include "Buzzer.h"
#include "Led_Controller.h"
#include "Lcd_Controller.h"

#include "Page.h"

#include "Critical_Page.h"
#include "Takeover_Page.h"
#include "Wheel_Speed_Page.h"
#include "Traction_Control_Page.h"

// Due to C++ polymorphism reasons, this MUST be an array of pointers
static Page* pages[NUM_PAGES];

static Takeover_Page takeover;
static Critical_Page critical;
static Traction_Control_Page traction_control;
static Wheel_Speed_Page wheel_speed;

static uint8_t curr_page = TAKEOVER_PAGE;

static String last_top = "                ";
static String last_bottom = "                ";

void Page_Manager::begin() {
  takeover = Takeover_Page();
  critical = Critical_Page();
  traction_control = Traction_Control_Page();
  wheel_speed = Wheel_Speed_Page();
  pages[TAKEOVER_PAGE] = &takeover;
  pages[CRITICAL_PAGE] = &critical;
  pages[TRACTION_CONTROL_PAGE] = &traction_control;
  pages[WHEEL_SPEED_PAGE] = &traction_control;
  update_page();
}

void Page_Manager::display() {
  String top = "";
  String bottom = "";

  // Takeover page only displays if it's selected and is a takeover
  if (curr_page == TAKEOVER_PAGE && takeover.shouldDisplay()) {
    takeover.screen(top, bottom);
  }

  // Critical page displays if takeover is selected and no takeover active
  else if (curr_page == TAKEOVER_PAGE || curr_page == CRITICAL_PAGE){
    critical.screen(top, bottom);
  }

  // TODO we may or may not need this, but is nice for linker to be able
  // to check compilation of each page's screen method
  else if (curr_page == TRACTION_CONTROL_PAGE) {
    traction_control.screen(top, bottom);
  }
  else if (curr_page == WHEEL_SPEED_PAGE) {
    wheel_speed.screen(top, bottom);
  }
  else {
    // TODO
    // pages[curr_page]->screen(top, bottom);
  }

  if (!top.equals(last_top)) {
    last_top = top;
    Lcd_Controller::writeMessage(last_top, 0, 0);
  }
  if (!bottom.equals(last_bottom)) {
    last_bottom = bottom;
    Lcd_Controller::writeMessage(last_bottom, 0, 1);
  }
}

void Page_Manager::process_action(Button_Action_T button_action) {
  Button_T button = button_action.button;
  Action_T action = button_action.action;
  if (button == DASH_RIGHT_BUTTON) {
    if (action == TAP) {
      Serial.println("NEXT");
      next_page();
    }
    else if (action == HOLD) {
      Serial.println("PREV");
      previous_page();
    }
  }
  else if (button == DASH_LEFT_BUTTON) {
    pages[curr_page]->act(action);
  }
}

void Page_Manager::next_page() {
  curr_page = (curr_page == NUM_PAGES - 1) ? 0 : curr_page + 1;
  update_page();
}

void Page_Manager::previous_page() {
  curr_page = (curr_page == 0) ? NUM_PAGES - 1 : curr_page - 1;
  update_page();
}

void Page_Manager::update_page() {
  for (uint8_t i = 0; i < NUM_PAGES; i++) {
    if (i == curr_page) {
      pages[i]->open();
    } else {
      pages[i]->close();
    }
  }
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
    pages[i]->process_Vcu_DashHeartbeat(&msg);
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
      pages[i]->process_ ## name(&msg); \
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
