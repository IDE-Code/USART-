#ifndef _CCD_H_
#define _CCD_H_

#include "sys.h"

void CCD_Init(void);
void Dly_us(void);
void RD_TSL(void);

void slove_data(void);
void sendToPc(void);

char binToHex_low(u8 num);
char binToHex_high(u8 num);

void CCD(void);
#endif


