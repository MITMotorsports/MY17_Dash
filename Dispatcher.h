#ifndef _DISPATCHER_H_
#define _DISPATCHER_H_

class Dispatcher {
  public:
    static void step();
    static void begin();
    static void dispatch();
    static void displayPages();
  private:
    static void processCanInputs();
    static void processButtonInputs();
};

#endif // _DISPATCHER_H_
