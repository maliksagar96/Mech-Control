#include "userLib/common.h"
#include "userLib/init.h"
#include "userLib/pidController.h"
#include "userLib/movingArray.h"
#include <stdbool.h>
#include "angle.h"
#include "shoot.h"

int32_t maxPWM_throw = 10;							//Random maxPWM value
int32_t maxPWM_angle = 10;							//Random maxPWM value
int32_t minPWM_throw = 0;						//Minimum PWM value for the throw motor to move
int32_t minPWM_angle = 0;						//Minimum PWM value for the angle motor to move

int shoot = 0,load = 0,plane_angle = 0;			//UART packet value holders of the mechanism

//Interrupt routines prototype
void Timer0IntHandler(void);
void ISR_ANGLE(void);
void ISR_THROW(void);
void UARTIntHandler(void);

int main() {
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	initPIDController(throw_motor,0.5,0.0,0.0); //0.16
	initPIDController(angle_motor,250.0,0.0,0.0);
	IntMasterEnable();
	uart0Init();
	UARTFIFODisable(UART0_BASE);
//	uart1Init();
//	UARTFIFODisable(UART1_BASE);
	uart5Init();
	maxPWM_throw = SysCtlClockGet()/(PWMfrequency*8);
	maxPWM_angle = SysCtlClockGet()/(PWMfrequency*8);
	minPWM_throw = maxPWM_throw/23.0;
	minPWM_angle = 0;
	maxPWM_throw = maxPWM_throw * 0.75;	// 0.7 times for middle pole at 90 degree
	pwmInit();
	motorDirInit(angle_motor);
	encoderInit(ISR_ANGLE,angle_motor);
	motorDirInit(throw_motor);
	encoderInit(ISR_THROW,throw_motor);
	timerInit();
	while(1) {
/*		UART_OutDec(throw_counter,0);
		UARTCharPut(UART0_BASE,';');
		UART_OutDec(angle_counter,0);
		UARTCharPut(UART0_BASE,';');
		UART_OutDec(shoot,0);
		UARTCharPut(UART0_BASE,';');
		UART_OutDec(triggered,0);
		UARTCharPut(UART0_BASE,';');
		UART_OutDec(shootComplete,0);
		UARTCharPut(UART0_BASE,';');
		UART_OutDec(load,0);
		UARTCharPut(UART0_BASE,';');
		UART_OutDec(plane_angle,0);
		UARTCharPut(UART0_BASE,0x0D);*/
	}
}

void Timer0IntHandler(void) {
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	shootDisc();
	change_angle();
}

void UARTIntHandler(void) {
	uint32_t ui32Status;
	ui32Status = UARTIntStatus(UART5_BASE, true); //get interrupt status
	UARTIntClear(UART5_BASE, ui32Status); //clear the asserted interrupts
	//uint32_t data = UARTCharGetNonBlocking(UART5_BASE);
	char data = UARTCharGet(UART5_BASE);//HWREG(UART5_BASE + UART_O_DR);
	UART_OutDec(data,0);
	UARTCharPut(UART0_BASE,0x0D);
	uint8_t temp_plane_angle;
	shoot = (data & 0b10000000);
	load = (data & 0b01000000);
	temp_plane_angle = (data & 0b00110000)>>4;
	switch(temp_plane_angle) {
	case 1:
		plane_angle++;
		if(plane_angle > 15) {
			plane_angle = 15;
		}
		break;
	case 2:
		plane_angle--;
		if(plane_angle < -15) {
			plane_angle = -15;
		}
		break;
	}
	if (shoot == SHOOT_DISK) {
		if (shootComplete == 1 && triggered == 0)
		{
			cmd_throw();
			triggered = 1;
			shootComplete = 0;
			steady_state_counter = 0;
		}
	} else if (shoot == 0) {
		triggered=0;
	}
	cmd_angle(plane_angle);
}

void ISR_ANGLE(void) {
	GPIOIntClear(ANGLE_ENCODER_PORT,ANGLE_ENCODER_CHANNELB);
	if(GPIOPinRead(ANGLE_ENCODER_PORT,ANGLE_ENCODER_CHANNELA)) {
		angle_counter++;
	} else {
		angle_counter--;
	}
}

void ISR_THROW(void) {
	GPIOIntClear(THROW_ENCODER_PORT,THROW_ENCODER_CHANNELB);
	if(GPIOPinRead(THROW_ENCODER_PORT,THROW_ENCODER_CHANNELA)) {
		throw_counter++;
	} else {
		throw_counter--;
	}
}
