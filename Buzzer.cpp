#include "Buzzer.h"
#include "Pins.h"

static bool enabled = true;

void Buzzer::begin() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

boolean buzzerOff(Task*) {
  Serial.println("Buzzer turned off");
  digitalWrite(BUZZER_PIN, LOW);
  // False means don't execute follow-up task
  return false;
}

DelayRun buzzerOffTask(1333, buzzerOff);

void Buzzer::trigger(int ms) {
  // If the noise is annoying, don't do it
  if(!enabled) {
    return;
  }

  Serial.println("Buzzer turned on");
  digitalWrite(BUZZER_PIN, HIGH);
  buzzerOffTask.setPeriodMs(ms);
  buzzerOffTask.startDelayed();
}
