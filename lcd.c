#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "lcdpcf8574/lcdpcf8574.h"

#define __DELAY_BACKWARD_COMPATIBLE__

/* This is the Design By Contract macros.*/
#define DBC // Can turn off these macros by commenting out this line
#ifdef DBC
/* needs to be at main since we are going to use Pin13 as our LED to warn us on assert fails */
#define DBC_SETUP() \
	/* turn on Pin 13 as we will use to indicate assertion/error failed */ \
	DDRB |= _BV(DDB5); 

#define PRE_CONDITION_DBC(eval_expression, time_blink_delay) \
	while (!(eval_expression))  \
	{ \
		PORTB |= _BV(PORTB5); \
		my_delay_ms(time_blink_delay); \
		PORTB &= ~_BV(PORTB5); \
		my_delay_ms(time_blink_delay); \
	}

#define POST_CONDITION_DBC(eval_expression, time_blink_delay) \
	while (!(eval_expression))  \
	{ \
		PORTB |= _BV(PORTB5); \
		my_delay_ms(time_blink_delay); \
		PORTB &= ~_BV(PORTB5); \
		/* half the delay off on post condition */ \
		my_delay_ms(time_blink_delay/2); \
	}
#elif
/* These are empty for when turned off */
#define DBC_SETUP() {}
#define PRE_CONDITION(eval_expression, time_blink_delay) {}
#define POST_CONDITION(eval_expression, time_blink_delay) {}
#endif

#define SECOND_IN_ms 5000
#define MY_BUTTON1 PD7 
#define MY_BUTTON2 PD6 

/* C Prototypes of functions */
void my_delay_ms(unsigned int delay);
short check_button_press(int button);
short check_button_press_and_release(int button);

int main(void)
{
    /* setup design by contract */
    DBC_SETUP();

    //init lcd, display on, cursor off
    lcd_init(LCD_DISP_ON);

    // set illumination
    uint8_t led = 0;
    lcd_led(led); //set led

	uint8_t line;
	while (1)
	{	
		// some special effects
		lcd_gotoxy(0, 0);
		lcd_puts("Welcome to ECE387");
		lcd_clear(0);
		lcd_gotoxy(0, 1);
		lcd_puts("Midterm Demo");
		lcd_clear(1);
		lcd_gotoxy(0, 2);
		lcd_puts("I2C 20x04 LCD");
		lcd_clear(2);
		lcd_gotoxy(0, 3);
		lcd_puts("Yicheng Qian");
		lcd_clear(3);
		lcd_gotoxy(0, 0);
		lcd_puts("Welcome to ECE387");
		lcd_gotoxy(0, 1);
		lcd_puts("Midterm Demo");
		lcd_gotoxy(0, 2);
		lcd_puts("I2C 20x04 LCD");
		lcd_gotoxy(0, 3);
		lcd_puts("Yicheng Qian");
		my_delay_ms(SECOND_IN_ms);
		// default menu goes here
		// clear the second line
		lcd_clrscr();
		for (line = 1; line < 3; line++)
		{
			lcd_gotoxy(0, line);
			if (line == 1)
			{
				lcd_puts("1: Clear Screen");
			}
			else if (line == 2)
			{
				lcd_puts("2: Messages");
			}
		}

		// button functions goes here
		// reset everything
		int btn2_counter = 0;
		int btn2_function = 1;
		// printing all required information or back to menu
		while (1) {
			if (check_button_press_and_release(MY_BUTTON2)) {
				btn2_counter++;
				if (btn2_function == 1) {
					lcd_clear(2);
					lcd_gotoxy(0, 2);
					lcd_puts("Yicheng Qian");
					btn2_function++;
				}
				else if (btn2_function == 2) {
					lcd_clear(2);
					lcd_gotoxy(0, 2);
					lcd_puti(btn2_counter);
					lcd_puts(" times");
					btn2_function++;
				}
				else if (btn2_function == 3) {
					lcd_clear(2);
					lcd_gotoxy(0, 2);
					lcd_puts("ECE 387 Sec A");
					btn2_function = 1;
				}
			}
			// if button 1 is pressed, then reset
			if (check_button_press_and_release(MY_BUTTON1))
				break;
		}
	}
	//while(1);
	return 0;
}

/*
 * checks when a button on the D port is pressed assuming a pull-down in non-pressed state
 *
 * WIRING: input and resitor on same connection, Vcc on other connection
 */
short check_button_press_and_release(int button)
{
	int ret_val = 0;

	PRE_CONDITION_DBC(button >= 0, 6000);
	PRE_CONDITION_DBC(button < 8, 7000);

	if ((PIND & (1 << button)) != 0)
	{
		/* software debounce */
		_delay_ms(15);
		if ((PIND & (1 << button)) != 0)
		{
			/* wait for button to be released */
			while ((PIND & (1 << button)) != 0)
				ret_val = 1;
		}
	}

	POST_CONDITION_DBC(ret_val == 1 || ret_val == 0, 5000);

	return ret_val;
}

/*
 * checks when a button on the D port is pressed assuming a pull-down in non-pressed state
 *
 * WIRING: input and resitor on same connection, Vcc on other connection
 */
short check_button_press(int button)
{
	int ret_val = 0;

	PRE_CONDITION_DBC(button >= 0, 6000);
	PRE_CONDITION_DBC(button < 8, 7000);

	if ((PIND & (1 << button)) != 0)
	{
		/* software debounce */
		_delay_ms(15);
		if ((PIND & (1 << button)) != 0)
		{
			ret_val = 1;
		}
	}

	POST_CONDITION_DBC(ret_val == 1 || ret_val == 0, 5000);

	return ret_val;
}

/*
 * Handles larger delays the _delay_ms can't do by itself (not sure how accurate)
 * Note no DBC as this function is used in the DBC !!!
 *
 * borrowed from : https://www.avrfreaks.net/forum/delayms-problem
 * */
void my_delay_ms(unsigned int delay)
{
	unsigned int i;

	for (i = 0; i < (delay / 10); i++)
	{
		_delay_ms(10);
	}
	for (i = 0; i < delay % 10; i++)
	{
		_delay_ms(1);
	}
}

