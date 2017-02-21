#include "Rtd_Handler.h"

// Non-member variable used in timer function pointers
bool enableFired = false;

const int MOTOR_TEMP_MODIFIER = 74; //0x4A

Debouncer debouncer(RTD_BUTTON_PIN, MODE_CLOSE_ON_PUSH, pressRtdButton, releaseRtdButton);

const int16_t SENTINAL = -32767;

Rtd_Handler::Rtd_Handler()
  : temps{SENTINAL, SENTINAL}
{
  // Initialization done above
}

void Rtd_Handler::begin() {
  pinMode(RTD_BUTTON_PIN, INPUT);
  PciManager.registerListener(RTD_BUTTON_PIN, &debouncer);
}

void sendEnableRequest() {
  Frame enableMessage = { .id=DASH_ID, .body={1}, .len=1};
  CAN().write(enableMessage);
}

void sendDisableRequest() {
  Frame disableMessage = { .id=DASH_ID, .body={0}, .len=1};
  CAN().write(disableMessage);
}

bool sendEnableRequestWrapper(Task*) {
  sendEnableRequest();
  enableFired = true;
  return false;
}

DelayRun sendEnableRequestTask(500, sendEnableRequestWrapper);

void pressRtdButton() {
  // The enable task will fire automatically if held for >1000ms
  enableFired = false;
  sendEnableRequestTask.startDelayed();
}

void releaseRtdButton(unsigned long) {
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

void Rtd_Handler::handleMessage(Frame& frame) {
  switch(frame.id) {
    case VCU_ID:
      processVcuMessage(frame);
      break;
    case BMS_SOC_ID:
      processSocMessage(frame);
      break;
    case POSITIVE_MOTOR_ID:
    case NEGATIVE_MOTOR_ID:
      processTempMessage(frame);
      break;
  }
}

void Rtd_Handler::processVcuMessage(Frame& message) {
  if (message.body[0] == 0) {
    // Blink the light
    RTD().disable();
  }
  else if (message.body[0] == 1) {
    // Make light solid
    RTD().enable();
  }
  else if (message.body[0] == 2) {
    // Turn off light
    RTD().shutdown();
  }
  else if (message.body[0] == 3) {
    // Overheat goes off
    LED().set_lightbar_fault(false);
  }
  else if (message.body[0] == 4) {
    // Overheat goes on
    LED().set_lightbar_fault(true);
  }
  else {
    // Should never happen
    Serial.println(F("Should never happen"));
  }
}

int16_t Rtd_Handler::mergeBytes(uint8_t low, uint8_t high) {
  uint16_t low_ext = low & 0x00FF;
  uint16_t high_ext = (high << 8);
  return high_ext | low_ext;
}

void Rtd_Handler::processTempMessage(Frame& message) {
  if(message.body[0] != MOTOR_TEMP_MODIFIER) {
    return;
  }
  uint8_t hi_order = message.body[2];
  uint8_t low_order = message.body[1];
  int16_t temp = mergeBytes(low_order, hi_order);
  temp = maxTemp(message, temp);

  // See http://i.imgur.com/vFoPyEU.png for explanation of conversions.
  // It isn't exactly linear but whatever close enough
  //
  // 17250 = 20 degrees c
  // 18250 = 30 degrees c
  // 19250 = 40 degrees c
  // 20250 = 50 degrees c
  const int16_t min_temp = 17250;
  const int16_t max_temp = 20250;

  if (temp < min_temp) {
    temp = min_temp;
  }
  if (temp > max_temp) {
    temp = max_temp;
  }
  temp = temp - min_temp;

  // Next scale from [0:3000] to [0:30]
  uint8_t scaled_temp = temp / 100;
  LED().set_lightbar_temperature(scaled_temp);
}

int16_t Rtd_Handler::maxTemp(Frame& message, int16_t motor_temp) {
  Motor this_motor;
  Motor other_motor;

  if (message.id == POSITIVE_MOTOR_ID) {
    this_motor = LeftMotor;
    other_motor = RightMotor;
  }
  else {
    this_motor = RightMotor;
    other_motor = LeftMotor;
  }

  // Store most recent temp reading for temp maxing
  temps[this_motor] = motor_temp;

  int16_t max_temp = max(temps[this_motor], temps[other_motor]);

  return max_temp;
}

void Rtd_Handler::processSocMessage(Frame& frame) {
  unsigned char SOC = frame.body[0];
  // Scale SOC from [0:100] to [0:30]
  double scaling_factor = 3.33333;
  SOC = SOC / scaling_factor;
  LED().set_lightbar_battery(SOC);
}
