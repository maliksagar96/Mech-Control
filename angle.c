#include "angle.h"
//Angle change variables
long int angle_counter = 0;
long int des_angle_counter = 0;

int convertAngleToTicks(float angle) {
	return (angle*TICK_PER_DEGREE);
}

void change_angle(void) {
	float error = des_angle_counter - angle_counter;
	float out = PID(angle_motor,error);
	if(fabs(out) <= ANGLE_TOLERANCE)
	{
		setPWM(0,angle_motor);
	} else {
		setPWM(out,angle_motor);
	}
}

void cmd_angle(int angle) {
	des_angle_counter = convertAngleToTicks(angle);
}
