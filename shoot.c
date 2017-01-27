/*
 * shoot.c
 *
 *  Created on: Dec 15, 2016
 *      Author: Aniket
 */
#include "shoot.h"
//10150 maxpWM/30 0.1
int shootComplete = 1,triggered = 0;
long int throw_counter = 6000;
long int des_throw_counter = 6000, FIRST_STAGE = 2000;
int steady_state_counter = 0;
bool steady = false;

void shootDisc(void) {
	if(throw_counter <= FIRST_STAGE) {
		setPWM(maxPWM_throw,throw_motor);
	}
	else if(throw_counter>FIRST_STAGE && throw_counter<=(FIRST_STAGE  + 2000)) {
		setPWM(maxPWM_throw/4,throw_motor);
	} else {
		float error = (des_throw_counter-throw_counter);
		float pwm = PID(throw_motor,error);
		if(absolute(error)<=SHOOT_TOLERANCE)
		{
			setPWM(0,throw_motor);
			if(steady == false) {
				steady_state_counter++;
			}
			if(steady_state_counter > STEADY_STATE_CONFIDENCE) {
				steady = true;
				shootComplete = 1;
			}
		} else {
			steady = false;
			steady_state_counter = 0;
			setPWM(pwm,throw_motor);
		}
	}
}

void cmd_throw(void) {
	if((des_throw_counter + STEP) < 0)
	{
		throw_counter = throw_counter - des_throw_counter + STEP;
	}
	des_throw_counter += STEP;
	FIRST_STAGE += STEP;
	if(des_throw_counter < 0) {
		des_throw_counter = STEP;
		FIRST_STAGE = 2000.0;
	}
}
