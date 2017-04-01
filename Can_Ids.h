#ifndef CAN_IDS_H
#define CAN_IDS_H

const int DASH_ID = 90; //0x05A
const int VCU_LED_ID = 82; //0x052
const int VCU_LIMP_ID = 1;//
const int VCU_AERO_ID = 2;
const int VCU_TC_ID = 3;
const int VCU_REGEN_ID = 4;
const int VCU_LV_ID = 5;

//byte index = which button is pushed, non zero values represent the state of the event
//byte 0 = rtd, byte 1 = limp
//1 = enable
//0 = no state changed
//2 = disable

#endif //CAN_IDS_H
