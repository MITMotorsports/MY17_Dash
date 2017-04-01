#ifndef CAN_IDS_H
#define CAN_IDS_H

const int DASH_ID = 90; //0x05A
const int VCU_ID = 82; //0x052

//byte index = which button is pushed, non zero values represent the state of the event
//byte 0 = rtd, byte 1 = limp
//1 = enable
//0 = no state changed
//2 = disable

#endif //CAN_IDS_H
