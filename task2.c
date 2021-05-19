#include "kmlib.h"
#include "km_tm4c123gh6pm.h"
extern int read[7];
extern int temp,hum;
void Task2(void)
{
	KM_LCD_Write_Cmd(0x80);
	KM_LCD_Write_Str(KM_itoa(read[2]));
	KM_LCD_Write_Str(":");
	KM_LCD_Write_Str(KM_itoa(read[1]));
	KM_LCD_Write_Str(":");
	KM_LCD_Write_Str(KM_itoa(read[0]));
	KM_LCD_Write_Str(" ");
	KM_LCD_Write_Str(KM_itoa(read[4]));
	KM_LCD_Write_Str("/");
	KM_LCD_Write_Str(KM_itoa(read[5]));
	KM_LCD_Write_Str("/");
	KM_LCD_Write_Str(KM_itoa(read[6]));
	KM_LCD_Write_Cmd( 0xc0 );
	KM_LCD_Write_Str("tem:");
	KM_LCD_Write_Str( KM_itoa(temp));
  KM_LCD_Write_Cmd( 0xc7 );
	KM_LCD_Write_Str("hum:");
	KM_LCD_Write_Str( KM_itoa(hum));
}