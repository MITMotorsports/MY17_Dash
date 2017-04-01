#include "Button_Listener.h"

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
  Frame enableMessage = { .id=DASH_ID, .body={1,0}, .len=2};
  CAN().write(enableMessage);
  Serial.println("sent enable request");
  enableFired = true;
}

void Button_Listener::sendDisableRequest() {
  Frame disableMessage = { .id=DASH_ID, .body={2,0}, .len=2};
  CAN().write(disableMessage);
  Serial.println("sent disable request");
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
    // Serial.println("Igothere");
    Button_Listener::releaseRtdButton();
  }

}

/*

void Button_Listener::sendEnableRequest() {
  Frame enableMessage = { .id=DASH_ID, .body={1,0}, .len=2};
  CAN().write(enableMessage);
  Serial.println("sent enable request");

}

void Button_Listener::sendDisableRequest() {
  Frame disableMessage = { .id=DASH_ID, .body={2,0}, .len=2};
  CAN().write(disableMessage);
  Serial.println("sent disable request");

}

boolean Button_Listener::sendEnableRequestWrapper(Task*) {
  sendEnableRequest();
  Serial.println("sendEnableRequest called");
  enableFired = true;
  return false;
}

DelayRun sendEnableRequestTask(500, Button_Listener::sendEnableRequestWrapper);

void Button_Listener::pressRtdButton() {
  // The enable task will fire automatically if held for >1000ms
  Serial.println("button press registered");
  enableFired = false;
  sendEnableRequestTask.startDelayed();
}

void Button_Listener::releaseRtdButton(unsigned long) {
  if(enableFired) {
    // Do nothing since car already enabled before release
    return;
  }
  else {
    // Button released before 500ms, so driver must want to disable
    SoftTimer.remove(&sendEnableRequestTask);
    sendDisableRequest();
  }
}
*/
