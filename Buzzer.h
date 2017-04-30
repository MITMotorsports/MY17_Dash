#ifndef BUZZER_H
#define BUZZER_H

#include <DelayRun.h>

const bool BUZZER_ENABLED = true;

class Buzzer {
  public:
    static void begin();
    static void trigger(int ms);
  private:
};

#endif // BUZZER_H
