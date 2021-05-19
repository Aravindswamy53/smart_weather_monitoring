#include "kmlib.h"
#include "km_tm4c123gh6pm.h"
//void *task(char* ,unsigned int );
void TIMER0_Init(void (*task)(void),unsigned long int period);
void TIMER1_Init(void (*task)(void),unsigned long int period);
void TIMER2_Init(void (*task)(void),unsigned long int period);
void Task0(void);
void Task1(void);
void Task2(void);
void Task3(void);
void Task4(void);
void TIMER0_Inter_Init(void);
void TIMER1_Inter_Init(void);
void TIMER2_Inter_Init(void);
void WaitForInterrupt();
int main()
{
	KM_Board_Init();
	TIMER0_Init(&Task0,2500000);
	TIMER0_Inter_Init();
	TIMER1_Init(&Task1,500000);
	TIMER1_Inter_Init();
	Task4();
	TIMER2_Init(&Task3,20000000);
	TIMER2_Inter_Init();
	Task2();
	while(1)
	{
		WaitForInterrupt();
		Task2();
	}
}