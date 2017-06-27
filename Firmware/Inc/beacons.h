#ifndef BEACONS_H__
#define BEACONS_H__

#include <stdint.h>
#include <stdbool.h>

void beacon_Init(void);

//TODO beacon API for rx beacon? to be called from IT?
bool beacon_Receive(uint16_t *rawBeacon);
void beacon_Send(uint16_t rawData);

uint32_t beacon_LastReceived(void);


#endif//BEACONS_H__

