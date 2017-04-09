#include "Button_Listener.h"

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
      if ( enableFired== false){
        rtdTimerID = rtdTimer.after(1000, Button_Listener::sendEnableRequest);
        // Serial.println(timerID);
      }
      else{
        //do nothing
      }
      break;
    case LIMP_BUTTON:
      if ( limpFired== false){
        limpTimerID = limpTimer.after(1000, Button_Listener::sendLimpLongRequest);
        // Serial.println(timerID);
      }
      else{
        //do nothing
      }
      break;
    case AERO_BUTTON:
      if ( limpFired== false){
        aeroTimerID = aeroTimer.after(1000, Button_Listener::sendAeroLongRequest);
        // Serial.println(timerID);
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

void Button_Listener::sendLimpLongRequest() {
    // Frame enableMessage = { .id=DASH_ID, .body={1,0}, .len=2};
    // CAN().write(enableMessage);
    Serial.println("sent limp long request");
    limpFired = true;
}

void Button_Listener::sendLimpShortRequest() {
    // Frame disableMessage = { .id=DASH_ID, .body={2,0}, .len=2};
    // CAN().write(disableMessage);
    Serial.println("sent limp short request");
    limpFired = false;
}
void Button_Listener::sendAeroLongRequest() {
    // Frame enableMessage = { .id=DASH_ID, .body={1,0}, .len=2};
    // CAN().write(enableMessage);
    Serial.println("sent limp long request");
    aeroFired = true;
}

void Button_Listener::sendAeroShortRequest() {
    // Frame disableMessage = { .id=DASH_ID, .body={2,0}, .len=2};
    // CAN().write(disableMessage);
    Serial.println("sent limp short request");
    aeroFired = false;
}

void Button_Listener::listen() {
  rtdDebounced.update();
  limpDebounced.update();
  aeroDebounced.update();
  rtdTimer.update();
  limpTimer.update();
  aeroTimer.update();
  //pin pulled down when button is pressed
  bool rtdPress = rtdDebounced.fell();
  bool rtdDepress = rtdDebounced.rose();
  //only when the button is depressed is when there is a full click
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
