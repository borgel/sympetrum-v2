#ifndef BEACONS_H__
#define BEACONS_H__

#include <stdint.h>

void beacon_Init(void);
void beacon_GiveTime(uint32_t systimeMS);

//TODO beacon API for rx beacon? to be called from IT?
void beacon_Receive(void);
void beacon_Send(void);

uint32_t beacon_LastReceived(void);


#endif//BEACONS_H__

