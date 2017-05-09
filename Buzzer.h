#ifndef BUZZER_H
#define BUZZER_H

#include <DelayRun.h>

class Buzzer {
  public:
    static void begin();
    static void trigger(int ms);
};

#endif // BUZZER_H
