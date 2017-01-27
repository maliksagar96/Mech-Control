/*
 * shoot.h
 *
 *  Created on: Dec 15, 2016
 *      Author: Aniket
 */

#ifndef SHOOT_H_
#define SHOOT_H_
#include "userLib/common.h"
#include "userLib/pidController.h"

#define STEP 6000.0
#define SHOOT_DISK 128
#define LOAD_DISK 64
#define SHOOT_TOLERANCE 5
#define STEADY_STATE_CONFIDENCE 50

extern int shootComplete,triggered;
extern long int throw_counter;
extern long int des_throw_counter, FIRST_STAGE;
extern int steady_state_counter;
extern bool steady;

void shootDisc(void);
void cmd_throw(void);

#endif /* SHOOT_H_ */
