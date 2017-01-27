/*
 * init.h
 *
 *  Created on: May 11, 2016
 *      Author: Aniket
 */

#ifndef init
#define init

#include "common.h"

void motorDirInit(int i);
void encoderInit(void (*pfHandler)(void),int i);
void timerInit(void);
void uart0Init(void);
void uart1Init(void);
void uart5Init(void);
void pwmInit(void);

#endif
