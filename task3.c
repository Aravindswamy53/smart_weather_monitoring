#include "kmlib.h"
#include "km_tm4c123gh6pm.h"
#include<stdio.h>
void (*PeriodicTask3)(void);
extern int temp,hum;
void EnableInterrupts();
void TIMER2A_Handler()
{
	if((TIMER2_MIS_R &(0x1))==1)
	{
		(*PeriodicTask3)();
	}
	TIMER2_ICR_R |= (0x1);
}
void TIMER2_Inter_Init(void)
{
	TIMER2_IMR_R |= TIMER_RIS_TATORIS; //Enable Interrupt for TATORIS bit
	NVIC_EN0_R |= 0x01<<23;		//Enable TIMER0A Interrupt for NVIC
	NVIC_PRI5_R &= 0xFFFF1FFF;		//Clear priority bits for Interrupt 19
	NVIC_PRI5_R |= 0x0000A000;		//Set priority bits to 0x05
	EnableInterrupts();
}
void TIMER2_Init(void (*task)(void),unsigned long int period)
{
	SYSCTL_RCGCTIMER_R |= 0x04;		//Enable clk of TIMER0
	//Wait for Peripheral ready is not required for Software Simulation
	//while(!(SYSCTL_PRTIMER_R & 0x01));		//Wait until Peripheral is ready
	PeriodicTask3 = task;
	TIMER2_CTL_R &=~(TIMER_CTL_TAEN);		//Clear TAEN bit to Disable TIMER
	TIMER2_CFG_R |= TIMER_CFG_32_BIT_TIMER;		//Set 0-2 bit fields to 000 for 32-bit TIMER
	TIMER2_TAMR_R |= TIMER_TAMR_TAMR_PERIOD;		//Set TAMR bit fields to 0x02 for Periodic Timer Mode
	TIMER2_TAILR_R = period-1;		//Load Period value for appropriate delay
	TIMER2_CTL_R |= TIMER_CTL_TAEN;		//Set TAEN bit to Enable TIMER
}
int set,flag=0;
char buffer[10];
char Fetch[50];
int wifi_response()
{
int i=0,set=0;
while(1)
{
buffer[i]=KM_UART2_InChar();
if(buffer[i]=='K' && buffer[i-1]=='O')
{
return 1;
}
if(i==9)
{++i;}
}
}
void wifi()
{
	int i;
	KM_UART2_Config(115200);
	KM_UART2_OutString ("AT\r\n");
	set=wifi_response();
	if(set==1)
	{
	KM_UART2_OutString("AT+CWMODE=3\r\n");
	set=wifi_response();
	if(set==1)
	{
		KM_UART2_OutString("AT+CWJAP=\"KernelMasters\",\"9963111084\"\r\n");
		set=wifi_response();
		if(set==1)
		{
			KM_UART2_OutString("AT+CIFSR\r\n");
			set=wifi_response();
			if(set==1)
			{
				KM_UART2_OutString("AT+CIPMODE=0\r\n");
				set=wifi_response();
				if(set==1)
				{
					flag=1;
				}  
			}
		}
	}
	}
}
void send_data_to_wifi()
{
	int i;
	KM_UART2_OutString("AT+CIPSTATUS\r\n");
	set=wifi_response();
  if(set==1)
		{
			KM_UART2_OutString("AT+CIPSTART=\"TCP\",\"142.93.218.33\",80\r\n");
			set=wifi_response();
      if(set==1)
         {
					 sprintf(Fetch,"GET /page.php?temp=%d&hum=%d&dev=1&kmid=km36aesd40\r\n\r\n",temp,hum);
           KM_UART2_OutString("AT+CIPSEND=60\r\n");
           for(i=0;i<2;++i)
             KM_delay_ms(250);
					 KM_UART2_OutString(Fetch);
                 // for(i=0;i<20;++i)
                //KM_delay_ms(200);
           KM_UART2_OutString("AT+CIPCLOSE\r\n");
                  //for(i=0;i<20;++i)
               //KM_delay_ms(200);   
					}
		}
}
void Task4(void)
{
	while(flag==0)
	{
		wifi();
	}	
}
void Task3(void)
{
	if(flag==1)
	{
	send_data_to_wifi();
	}
}