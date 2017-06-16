#include "beacons.h"
#include "iprintf.h"

#include "ir_encode.h"
#include "ir_decode.h"

#include <stdint.h>
#include <string.h>

struct beacon_State {
   uint32_t    beaconIntervalMS;
   uint32_t    lastSent;

   // systime timestamp from the last time we got a packet
   uint32_t    lastReceived;
};

static struct beacon_State state;

void beacon_Init(uint32_t intervalMS) {
   memset(&state, 0, sizeof(state));

   state.beaconIntervalMS = intervalMS;

   iprintf("Setting up RC5 encode/decode...");
   ir_InitEncode();
   ir_InitDecode();
   iprintf("ok\r\n");
}

void beacon_GiveTime(uint32_t systimeMS) {
   //TODO is there a better way to check or data?
   beacon_Receive();

   //if interval has expired, beacon
   if(systimeMS - state.lastSent > state.beaconIntervalMS) {
      beacon_Send();

      state.lastSent = systimeMS;
   }
}

void beacon_Send(void) {
   ir_DecodeDisable();

   //TODO what do we send?
   //ir_SendRC5(4, 23, RC5_Ctrl_Reset);
   ir_SendRaw(0x0EEF);

   //FIXME remove to unblock?
   while(ir_IsSending()) {}

   ir_DecodeEnable();
}

//TODO what do we connect this to? IT?
void beacon_Receive(void) {
   uint16_t rawFrame;
   RC5_Frame_TypeDef rcf;

   if(ir_GetDecoded(&rawFrame, &rcf)) {
      iprintf("Raw  0x%x\r\n", rawFrame);
      iprintf("Addr   %d\r\n", rcf.Address);
      iprintf("Comd   %d\r\n", rcf.Command);
      iprintf("Field  %d\r\n", rcf.FieldBit);
      iprintf("Toggle %d\r\n", rcf.ToggleBit);
      iprintf("\r\n");

      state.lastReceived = HAL_GetTick();
   }
}

uint32_t beacon_LastReceived(void) {
   return state.lastReceived;
}
