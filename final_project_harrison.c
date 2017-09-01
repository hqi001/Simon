/*
hqi001_finalProject.c
 *
 * Harrison Qi, hqi001@ucr.edu
 * Lab Section - B21
 * Assignment: Final Project: Simon
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 * GccApplication1.c
 *
 * Created: 8/23/2017 1:20:46 PM
 *  Author: student
 */ 

/*
Simon
LCD welcome screen
Wait for button press to begin
Random sequence shown on led
Player enters sequence on button presses
Report score on LCD screen
Winning sequence is 9 button presses
Report win/lose on LCD screen
Bonus:  Add sounds with the LED/Button press
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "io.c"

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

unsigned char button;
unsigned char start;
unsigned int x;


void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHseed clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialiseede avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR()
{
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

unsigned char GetBit (unsigned char x, unsigned char k ) {
	return ((x & (0x01 << k)) != 0);
}


void set_PWM(double frequency) {
	static double current_frequency;
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; }
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		else if (frequency > 31250) { OCR3A = 0x0000; }
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }
		TCNT3 = 0;
		current_frequency = frequency;
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}
//------------------------------------------------------------

unsigned char score;
double store_array[9] = {};
unsigned char i;
unsigned char j;

unsigned char wait_input;

void assign_Inputs(unsigned char score_int){
	store_array[score_int] = rand() % 4;
	
}

enum welcome_state 
{
	init_welcome, 
	waiting,
	game_start,	
} welcome_states;

unsigned char button = 0x00;
unsigned char score;
unsigned char seed;

void welcome_screen()
{
	button = ~PINA & 0x10;
	
	switch(welcome_states) 
	{
		case init_welcome:
			PORTB = 0x00;
			LCD_init();
			LCD_DisplayString(1, "Press button to start");
			welcome_states = waiting;
			break;
			
		case waiting:
			if(!button)
			{
				welcome_states = waiting;
			}
			else 
			{
				welcome_states = game_start;
			}
			break;
			
		case game_start:
			start = 1;
			srand(seed);
			seed++;
			assign_Inputs(score);
			LCD_init();
			LCD_DisplayString(1, "Start!");
			welcome_states = init_welcome;
			break;
			
		default:
			break;
	}
}

unsigned char store = 0;
unsigned char lose;
unsigned char win;

void read_input() //checks for user correct input
{
	button = ~PINA & 0x0F;
	PWM_on();
	set_PWM(0);
		if(button == 0x01)
		{
			store = 0;
			PORTB = button;
			set_PWM(261.63);
			if(store == store_array[j])
			{
				++j;
			}
			else
			{
				lose = 1;
			}
		}
		else if(button == 0x02)
		{
			store = 1;
			PORTB = button;
			set_PWM(329.63);
			if(store == store_array[j])
			{
				++j;
			}
			else
			{
				lose = 1;
			}
		}
		else if(button == 0x04)
		{
			store = 2;
			PORTB = button;
			set_PWM(392.00);
			if(store == store_array[j])
			{
				++j;
			}
			else
			{
				lose = 1;
			}
		}
		else if(button == 0x08)
		{
			store = 3;
			PORTB = button;
			set_PWM(493.88);
			if(store == store_array[j])
			{
				++j;
			}
			else
			{
				lose = 1;
			}
		}
		
		if(j == i)
		{
			wait_input = 0;
			i = 0;
			score++;
			
			if(!(score == 9))
			{
				assign_Inputs(score);
			}

			if(score == 9)
			{
				win = 1;
			}
			else
			{
				LCD_init();
				LCD_DisplayString(1, "Score: ");
				LCD_WriteData(score + '0');
			}
		}
	
	
}

enum light_state
{
	init_write,
	blue,
	yellow,
	green,
	red,
	OFF,
} light_states;

unsigned double tmpB;

void start_game()
{
	tmpB = store_array[i];
	switch(light_states)
	{
		case init_write:
			PORTB = 0x00;
			if(i<(score+1))
			{
				if (tmpB == 0))
				{
					light_states = blue;
					i++;
					break;
				}
				else if (tmpB == 1))
				{
					light_states = yellow;
					i++;
					break;
				}
				else if ((tmpB == 2))
				{
					light_states = green;
					i++;
					break;
				}
				else if (tmpB == 3))
				{
					light_states = red;
					i++;
					break;
				}
			}
			else if(i == (score+1))
			{
				light_states = OFF;
				break;
			}
			break;
		
		case blue:
			set_PWM(261.63);
			PORTB = 0x01;
			light_states = init_write;
			break;
		
		case yellow:
			set_PWM(329.63);
			PORTB = 0x02;
			light_states = init_write;
			break;
		
		case green:
			set_PWM(392.00);
			PORTB = 0x04;
			light_states = init_write;
			break;
		
		case red:
			set_PWM(493.88);
			PORTB = 0x08;
			light_states = init_write;
			break;
			
		case OFF:
			set_PWM(0);
			PORTB = 0x00;
			wait_input = 1;
			j = 0;
			light_states = init_write;
			break;
	}
}

enum lose_state
{
	init_lose,
	wait_lose1,
	wait_lose2,
	wait_lose3,
	wait_lose4,
	wait_lose5,
	wait_lose6,
} lose_states;
	
void lose_screen()
{
	switch(lose_states)
	{
	
		case init_lose:
			set_PWM(0);
			LCD_init();
			LCD_DisplayString(1, "You lose.. Final score: ");
			LCD_WriteData(score + '0');
			lose_states = wait_lose1;
			break;
			
		case wait_lose1:
			lose_states = wait_lose2;
			break;
			
		case wait_lose2:
			lose_states = wait_lose3;
			break;
		
		case wait_lose3:
			lose_states = wait_lose4;
			break;
		
		case wait_lose4:
			lose_states = wait_lose5;
			break;
		
		case wait_lose5:
			lose_states = wait_lose6;
			break;
		
		case wait_lose6:
			i = 0;
			j = 0;
			start = 0;
			wait_input = 0;
			score = 0;
			lose = 0;
			lose_states = init_lose;
			break;
	}
	
}

enum win_state
{
	init_win,
	wait_win1,
	wait_win2,
	wait_win3,
	wait_win4,
	wait_win5,
	wait_win6,
} win_states;

void win_screen()
{
	switch(win_states)
	{
		
		case init_win:
			set_PWM(0);
			LCD_init();
			LCD_DisplayString(1, "You win! Final  score: ");
			LCD_WriteData(score + '0');
			win_states = wait_win1;
			break;
		
		case wait_win1:
			win_states = wait_win2;
			break;
		
		case wait_win2:
			win_states = wait_win3;
			break;
		
		case wait_win3:
			win_states = wait_win4;
			break;
		
		case wait_win4:
			win_states = wait_win5;
			break;
		
		case wait_win5:
			win_states = wait_win6;
			break;
		
		case wait_win6:
			i = 0;
			j = 0;
			start = 0;
			wait_input = 0;
			score = 0;
			win = 0;
			win_states = init_win;
			break;
	}
	
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	unsigned long welcome_elapsedTime = 200;
	unsigned long start_elapsedTime = 200;
	unsigned long input_elapsedTime = 200;
	unsigned long lose_elapsedTime = 500;
	unsigned long win_elapsedTime = 500;
	const unsigned long timerPeriod = 100;
	TimerSet(timerPeriod);
	TimerOn();
	PWM_on();
	i = 0;
	j = 0;
	start = 0;
	button = 0;
	wait_input = 0;
	score = 0;
	lose = 0;
	win = 0;
	seed = 4;
	light_states = init_write;
	welcome_states = init_welcome;
	lose_states = init_lose;
	win_states = init_win;

	while(1)
	{
		if(welcome_elapsedTime >=200)
		{
			if(!start && !lose && !win)
			{
				welcome_screen();
			}
			welcome_elapsedTime = 0;
		}
		if(start_elapsedTime >=500)
		{
			if(start && !wait_input && !lose && !win)
			{
				PWM_on();
				set_PWM(0);
				start_game();
			}
			start_elapsedTime = 0;
		}
		if(input_elapsedTime >=200)
		{
			if(start && wait_input && !lose && !win)
			{
				read_input();
			}
			input_elapsedTime = 0;
		}
		if(lose_elapsedTime >= 500)
		{
			if(lose)
			{
				lose_screen();
			}
			lose_elapsedTime = 0;
		}
		if(win_elapsedTime >= 500)
		{
			if(win)
			{
				win_screen();
			}
			win_elapsedTime = 0;
		}
		while (!TimerFlag)
		{
			
		}
		TimerFlag = 0;
		lose_elapsedTime += timerPeriod;
		welcome_elapsedTime += timerPeriod;
		start_elapsedTime += timerPeriod;
		input_elapsedTime += timerPeriod;
		win_elapsedTime += timerPeriod;
	}
}