
#include "lcd.h"
#include "lm35_sensor.h"
#include <avr/delay.h>
#include <avr/io.h>

enum get_temp_value{
	OFF,ON
}temp_value;

void LCD_temp_module(uint8 temp)
{


	  	if (temp <30)
	  	{

	  		temp_value = OFF;
	  	}
	  	else if(temp>=30)
	  	{

	  		temp_value = ON;
	  	}
	  	//first row

	  	if (temp_value == ON)
	  	{
	  		LCD_displayStringRowColumn(0, 11, "ON ");

	  	}
	  	else
	  	{
	  		LCD_displayStringRowColumn(0, 11, "OFF");
	  	}


		LCD_moveCursor(1, 11);
	  	LCD_intgerToString(temp);
	  	if(temp<10)
	  	{
	  		LCD_displayString("  ");
	  	}
	  	else if(temp<100)
	  	{
	  		LCD_displayCharacter(' ');
	  	}

	  	//second row
	  	//LCD_moveCursor(1,8);

}
void Timer0_PWM_Init(unsigned char set_duty_cycle)
{
	TCNT0 = 0; // Set Timer Initial Value to 0

	OCR0  = set_duty_cycle; // Set Compare Value

	DDRB  = DDRB | (1<<PB3); // Configure PB3/OC0 as output pin --> pin where the PWM signal is generated from MC

	/* Configure timer control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
	 */
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
}

void temp_pwm_module(uint8 temp)
{

	  if(temp<30)
		  {

		  	  PORTB &= (0<<PB1);


		  }
		  else if(temp >= 30 && temp<60)
		  {

			  PORTB |= (1<<PB1);
			  Timer0_PWM_Init(64);
		  }
		  else if(temp >= 60 && temp <90)
		  {

			  PORTB |= (1<<PB1);
			  Timer0_PWM_Init(128);
		  }
		  else if (temp >=90 && temp<120)
		  {
			  PORTB |= (1<<PB1);
			  Timer0_PWM_Init(190);
		  }
		  else
		  {
			  PORTB |= (1<<PB1);
			  Timer0_PWM_Init(255);
		  }
}

int main(void)
{

	//GPIO_setupPinDirection(1,0, PIN_OUTPUT);
	//GPIO_setupPinDirection(1,1, PIN_OUTPUT);
	LCD_init();
	ADC_init();
	uint8 temp =0;
	DDRB |= 0x03;
	PORTB &= (~(1<<PB0));
	LCD_displayStringRowColumn(0, 4, "FAN is ");
	LCD_displayStringRowColumn(1, 4, "Temp =    C");

	while(1)
	{
	  temp = LM35_getTemperature();
	  LCD_temp_module(temp);
	  temp_pwm_module(temp);
	  //temp_pwmMotor

	}
}


