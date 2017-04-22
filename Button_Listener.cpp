#include "Button_Listener.h"

#include <MY17_Can_Library.h>

// Non-member variable used in timer function pointers
bool enableFired = false;
bool limpFired = false;
bool aeroFired = false;

// button mode is normally open, i.e. closes on push
// pressRtdButton is called on press
// releaseRtdButton is called on depress
Bounce aeroDebounced;
Bounce limpDebounced;
Bounce rtdDebounced;

Timer rtdTimer;
Timer limpTimer;
Timer aeroTimer;

int8_t rtdTimerID;
int8_t limpTimerID;
int8_t aeroTimerID;

void Button_Listener::begin() {
  pinMode(RTD_BUTTON_PIN, INPUT);
  pinMode(LIMP_BUTTON_PIN, INPUT);
  pinMode(AERO_BUTTON_PIN, INPUT);
  rtdDebounced.attach(RTD_BUTTON_PIN);
  limpDebounced.attach(LIMP_BUTTON_PIN);
  aeroDebounced.attach(AERO_BUTTON_PIN);
  rtdDebounced.interval(50);
  limpDebounced.interval(50);
  aeroDebounced.interval(50);
}

void Button_Listener::pressButton(ButtonName button){
  Serial.println("Button pressed");
  switch (button) {
    case RTD_BUTTON:
      if (enableFired == false){
        rtdTimerID = rtdTimer.after(500, Button_Listener::sendEnableRequest);
      }
      else{
        //do nothing
      }
      break;
    case LIMP_BUTTON:
      if ( limpFired== false){
        limpTimerID = limpTimer.after(500, Button_Listener::sendLimpLongRequest);
      }
      else{
        //do nothing
      }
      break;
    case AERO_BUTTON:
      if ( limpFired== false){
        aeroTimerID = aeroTimer.after(500, Button_Listener::sendAeroLongRequest);
      }
      else{
        //do nothing
      }
      break;
  }
}

void Button_Listener::releaseButton(ButtonName button){
  Serial.println("Button released");
  switch (button) {
    case RTD_BUTTON:
      rtdTimer.stop(rtdTimerID);
      if (enableFired) {
        //do nothing, rtd enable message has been sent, reset enableFired
        enableFired = false;
      }
      else {
        sendDisableRequest();
      }
      break;
    case LIMP_BUTTON:
      limpTimer.stop(limpTimerID);
      if (limpFired) {
        //do nothing, rtd enable message has been sent, reset enableFired
        limpFired = false;
      }
      else {
        sendLimpShortRequest();
      }
      break;
    case AERO_BUTTON:
      aeroTimer.stop(aeroTimerID);
      if (aeroFired) {
        //do nothing, rtd enable message has been sent, reset enableFired
        aeroFired = false;
      }
      else {
        sendAeroShortRequest();
      }
      break;
  }
}

void Button_Listener::sendEnableRequest() {
  Can_Dash_Request_T msg;
  msg.type = CAN_DASH_REQUEST_RTD_ENABLE;
  Can_Dash_Request_Write(&msg);
  enableFired = true;
}

void Button_Listener::sendDisableRequest() {
  Can_Dash_Request_T msg;
  msg.type = CAN_DASH_REQUEST_RTD_DISABLE;
  Can_Dash_Request_Write(&msg);
  enableFired = false;
}

void Button_Listener::sendLimpLongRequest() {
  Can_Dash_Request_T msg;
  msg.type = CAN_DASH_REQUEST_LIMP_MODE_ENABLE;
  Can_Dash_Request_Write(&msg);
  limpFired = true;
}

void Button_Listener::sendLimpShortRequest() {
  Can_Dash_Request_T msg;
  msg.type = CAN_DASH_REQUEST_LIMP_MODE_DISABLE;
  Can_Dash_Request_Write(&msg);
  limpFired = false;
}

void Button_Listener::sendAeroLongRequest() {
  Serial.println("sent aero long request");
  aeroFired = true;
}

void Button_Listener::sendAeroShortRequest() {
  Can_Dash_Request_T msg;
  msg.type = CAN_DASH_REQUEST_DATA_FLAG;
  Can_Dash_Request_Write(&msg);
  aeroFired = false;
}

void Button_Listener::listen() {
  rtdDebounced.update();
  limpDebounced.update();
  aeroDebounced.update();

  rtdTimer.update();
  limpTimer.update();
  aeroTimer.update();

  bool rtdPress = rtdDebounced.fell();
  bool rtdDepress = rtdDebounced.rose();

  bool limpPress = limpDebounced.fell();
  bool limpDepress = limpDebounced.rose();

  bool aeroPress = aeroDebounced.fell();
  bool aeroDepress = aeroDebounced.rose();

  if (rtdPress == true){
    pressButton(RTD_BUTTON);
  }
  else if (rtdDepress == true){
    releaseButton(RTD_BUTTON);
  }
  if(limpPress == true){
    pressButton(LIMP_BUTTON);
  }
  else if (limpDepress == true){
    releaseButton(LIMP_BUTTON);
  }
  if(aeroPress == true){
    pressButton(AERO_BUTTON);
  }
  else if (aeroDepress == true){
    releaseButton(AERO_BUTTON);
  }
}
