#include "Dash.h"

#include <Arduino.h>

#include "Dispatcher.h"

void setup() {
  // Dispatcher does heavy lifting
  Dispatcher::begin();
}

