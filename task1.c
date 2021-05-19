#include "km_tm4c123gh6pm.h"
#include "kmlib.h"
void EnableInterrupts();

int read[7];//rtc values

void I2C0_Config(void);//rtc pin initialization
void (*PeriodicTask1)(void); // Function pointer Declaration
void TIMER1_Init(void (*task)(void),unsigned long int period)
{
	SYSCTL_RCGCTIMER_R |= 0x02;		//Enable clk of TIMER1
	//Wait for Peripheral ready is not required for Software Simulation
	//while(!(SYSCTL_PRTIMER_R & 0x02));		//Wait until Peripheral is ready
	KM_I2C0_Init();
	I2C0_Config();
	PeriodicTask1 = task; // Function Pointer Initalization
	TIMER1_CTL_R &=~(TIMER_CTL_TAEN);		//Clear TAEN bit to Disable TIMER
	TIMER1_CFG_R |= TIMER_CFG_32_BIT_TIMER;		//Set 0-2 bit fields to 000 for 32-bit TIMER
	TIMER1_TAMR_R |= TIMER_TAMR_TAMR_PERIOD;		//Set TAMR bit fields to 0x02 for Periodic Timer Mode
	TIMER1_TAILR_R = period-1;		//Load Period value for appropriate delay
	TIMER1_CTL_R |= TIMER_CTL_TAEN;		//Set TAEN bit to Enable TIMER
}
void TIMER1_Inter_Init(void)
{
	TIMER1_IMR_R |= TIMER_RIS_TATORIS; //Enable Interrupt for TATORIS bit
	NVIC_EN0_R |= 0x01<<21;		//Enable TIMER1A Interrupt for NVIC
	NVIC_PRI5_R &= 0xFFFF1FFF;		//Clear priority bits for Interrupt 21
	NVIC_PRI5_R |= 0x00008000;		//Set priority bits to 0x07
	EnableInterrupts();
}
void TIMER1A_Handler(void)
{
	if(TIMER1_RIS_R & TIMER_RIS_TATORIS)		//If TATORIS bit is set
	{
		(*PeriodicTask1)();// Function Pointer CALLING
		TIMER1_ICR_R |= TIMER_RIS_TATORIS;		//Clear TATORIS in RIS reg
	}
}
void I2C0_Config(void)
{
	//GPIO_PORTB_ODR_R |=(0XC);
	I2C0_MCR_R|=0X10;
	I2C0_MTPR_R =0X18;
}
void RTC_Read(void)
{
	int i;
	I2C0_MSA_R=((0X68)<<1);
	I2C0_MSA_R &= ~(0x1);
	I2C0_MDR_R=0X00;
	I2C0_MCS_R=0X03;

	while(I2C0_MCS_R & 0x1);
	I2C0_MSA_R=((0X68)<<1);
	I2C0_MSA_R |=(0x1);
	I2C0_MCS_R=0X0B;
	while(I2C0_MCS_R & 0x1)
	{
		;
	}
	for(i=0;i<7;++i)
	{
			while(I2C0_MCS_R & 0x1);
			if(!(I2C0_MCS_R & (0x1<<1)))
			{
				read[i]=I2C0_MDR_R;
				read[i]=read[i]-6*(read[i]>>4);
				I2C0_MCS_R=0x9;//
			}
	}
	I2C0_MCS_R=0X5;
	while(I2C0_MCS_R & 0x1);
	while(I2C0_MCS_R & (0x1<<1));
}
int RTC_Write(void)
{
	int i;
	int write[7]={0X24,0X20,0X10,0X5,0X21,0X11,0X19};
	I2C0_MSA_R=((0X68)<<1);
	I2C0_MSA_R &= ~(0x1);
	I2C0_MDR_R=0X00;
	while(I2C0_MCS_R & 0x1);
	I2C0_MCS_R=0X3;
	for(i=0;i<7;)
	{
		while(I2C0_MCS_R & 0x1);
		if(!(I2C0_MCS_R & (0x1<<1)))
		{
			I2C0_MDR_R=write[i];
			I2C0_MCS_R=0X1;//RUN
			++i;
		}
	}
	I2C0_MCS_R=0X5;//STOP AND RUN
	while(I2C0_MCS_R & 0x1);
	return (I2C0_MCS_R&0x0E);
}
void Task1(void)
{
	if(RTC_Write()==0)
	{
		while(1)
		{
			RTC_Read();
		}			
	}
}
