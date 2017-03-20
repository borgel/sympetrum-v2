/******************************************************************************
*This file is for iprintf()
*The iprintf() is a simple printf() and only can print string with %s,%d,%c,%x.
*******************************************************************************/
#include "iprintf.h"
#include "stm32f0xx_hal.h"

#include <stdint.h>

extern UART_HandleTypeDef huart1;

static void iputc(char ch)
{
   //TODO what should this timeout be?
   HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, 10000);
}

static uint8_t* change(uint32_t Index)
{
    return (uint8_t*)("0123456789abcdef"+Index);
}


void iprintf(char *pszFmt,...)
{
    uint8_t *pszVal;
    uint32_t iVal, xVal, i = 0, buffer[12], index = 1;
    uint8_t cVal;
    uint32_t *pArg;
    pArg =(uint32_t*)&pszFmt;

    while(*pszFmt)
    {
        if('%' != *pszFmt)
        {
            iputc(*pszFmt);
            pszFmt++;
            continue;
        }
        pszFmt++;

        if(*pszFmt == 's')
        {
            pszVal = (uint8_t*)pArg[index++];
            for(; *pszVal != '\0'; pszVal++)
                iputc(*pszVal);
            pszFmt++;
            continue;
        }
        if(*pszFmt == 'd')
        {
            iVal = pArg[index++];
            i = 0;
            do{
                buffer[i++] = iVal % 10;
                iVal /= 10;
            }while(iVal);
            while(i > 0)
            {
                i--;
                iputc(*change(buffer[i]));
            }
            pszFmt++;
            continue;
        }
        if(*pszFmt == 'c')
        {
            cVal = (uint8_t)pArg[index++];
            iputc(cVal);
            pszFmt++;
            continue;
        }
        if(*pszFmt == 'x')
        {
            xVal = pArg[index++];
            i = 0;
            do{
                buffer[i++] = xVal % 16;
                xVal /= 16;
            }while(xVal);
            if(i%2!=0)
                buffer[i++]=0;
            if(i<2)
                buffer[i++]=0;

            while(i > 0)
            {
                i--;
                iputc(*change(buffer[i]));
            }
            pszFmt++;
            continue;
        }
        if(pszFmt == '\0')
        {
            break;
        }

    }
}
