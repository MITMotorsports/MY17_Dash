#include "Button_Listener.h"

#include <MY17_Can_Library.h>

// Non-member variable used in timer function pointers
bool enableFired = false;

// button mode is normally open, i.e. closes on push
// pressRtdButton is called on press
// releaseRtdButton is called on depress
Bounce rtdDebounced;
Timer rtdTimer;
Timer testTimer;
int8_t timerID;

void Button_Listener::begin() {
  pinMode(RTD_BUTTON_PIN, INPUT);
  rtdDebounced.attach(RTD_BUTTON_PIN);
  rtdDebounced.interval(50);
}

void Button_Listener::pressRtdButton(){
  Serial.println("Button pressed");
  if (enableFired == false){
    timerID = rtdTimer.after(1000,Button_Listener::sendEnableRequest);
    Serial.println(timerID);
  }
  else{
    //do nothing
  }
}

void Button_Listener::releaseRtdButton(){
  Serial.println("Button released");
  rtdTimer.stop(timerID);
  if (enableFired) {
    //do nothing, rtd enable message has been sent, reset enableFired
    enableFired = false;
  }
  else {
  Button_Listener::sendDisableRequest();
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

void Button_Listener::listen() {
  rtdDebounced.update();
  rtdTimer.update();
  //pin pulled down when button is pressed
  boolean rtdPress = rtdDebounced.fell();
  boolean rtdDepress = rtdDebounced.rose();
  if (rtdPress == true){
    Button_Listener::pressRtdButton();
  }
  else if (rtdDepress == true){
    Button_Listener::releaseRtdButton();
  }
}
