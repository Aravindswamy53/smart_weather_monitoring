#include "km_tm4c123gh6pm.h"
#include "kmlib.h"
void (*PeriodicTask0)(void);
int temp,hum;
void EnableInterrupts();
void TIMER0A_Handler()
{
	if((TIMER0_MIS_R &(0x1))==1)
	{
		(*PeriodicTask0)();
	}
	TIMER0_ICR_R |= (0x1);
}
void TIMER0_Inter_Init(void)
{
	TIMER0_IMR_R |= TIMER_RIS_TATORIS; //Enable Interrupt for TATORIS bit
	NVIC_EN0_R |= 0x01<<19;		//Enable TIMER0A Interrupt for NVIC
	NVIC_PRI4_R &= 0x1FFFFFFF;		//Clear priority bits for Interrupt 19
	NVIC_PRI4_R |= 0xA0000000;		//Set priority bits to 0x05
	EnableInterrupts();
}
void TIMER0_Init(void (*task)(void),unsigned long int period)
{
	SYSCTL_RCGCTIMER_R |= 0x01;		//Enable clk of TIMER0
	//Wait for Peripheral ready is not required for Software Simulation
	//while(!(SYSCTL_PRTIMER_R & 0x01));		//Wait until Peripheral is ready
	PeriodicTask0 = task;
	TIMER0_CTL_R &=~(TIMER_CTL_TAEN);		//Clear TAEN bit to Disable TIMER
	TIMER0_CFG_R |= TIMER_CFG_32_BIT_TIMER;		//Set 0-2 bit fields to 000 for 32-bit TIMER
	TIMER0_TAMR_R |= TIMER_TAMR_TAMR_PERIOD;		//Set TAMR bit fields to 0x02 for Periodic Timer Mode
	TIMER0_TAILR_R = period-1;		//Load Period value for appropriate delay
	TIMER0_CTL_R |= TIMER_CTL_TAEN;		//Set TAEN bit to Enable TIMER
}
void Task0(void)
{
	//GPIO_PORTF_DATA_R &= 0xFFFFFFF1;		//Clear LED's
	//GPIO_PORTF_DATA_R ^= 0x02;		//Toggle RED-LED
	int j=0,a,n1,n2,p,t;
	while(1)
	{ 
	a=1;
	loop :DHT11_Request();
	a= DHT11_Response();
		if(a!=0)
		{
		hum=DHT11_Rx_Data();
		n1=DHT11_Rx_Data();
		temp=DHT11_Rx_Data();
		n2=DHT11_Rx_Data();
		p=DHT11_Rx_Data();
		t=temp+n1+hum+n2;
			if(t!=p)
			{
				goto loop;
			}
		}
	}
}