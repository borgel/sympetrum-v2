#include "board_id.h"

//FIXME rm
#include "iprintf.h"

#include <stdint.h>

/**
 * Returns an approximate GUID (in STM32 land) based on a bunch of fabrication-related metadata.
 *
 * See Technical Reference Manual pg933 ("Unique device ID register") for address and explanation.
 * First 32 bits are wafer X Y coordinates in BCD
 * Next 8 are wafer number
 * Next 56 are lot number in ASCII
 */
#define UNIQUE_ID_REG_ADDR            0x1FFFF7AC
#define UNIQUE_ID_REG_GET8(x)        ((x >= 0 && x < 12) ? (*(uint8_t *) (UNIQUE_ID_REG_ADDR + (x))) : 0)

uint32_t bid_GetID(void) {
   int i;
   for(i = 0; i < 12; i++) {
      //FIXME rm printing
      iprintf("ID #%d - 0x%x (%c)\n", i, UNIQUE_ID_REG_GET8(i), UNIQUE_ID_REG_GET8(i));
   }

   // use wafer X/Y for ID. The tray I got all has the same lot number
   return   UNIQUE_ID_REG_GET8(0) << 0 |
            UNIQUE_ID_REG_GET8(1) << 8 |
            UNIQUE_ID_REG_GET8(2) << 16|
            UNIQUE_ID_REG_GET8(3) << 24;
}

