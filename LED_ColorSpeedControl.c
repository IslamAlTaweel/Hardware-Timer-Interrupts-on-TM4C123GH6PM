/*Implement an LED blinking program that cycles through multiple colors using two switches, where one switch changes the LED color and the other switch changes the blink speed*/
#include "TM4C123.h" 
#include "utils.h"

#define SRC_CLK 50000000

volatile int index = 0;

int seq[] = {RED, BLUE, GREEN, YELLOW, MAGENTA, CYAN, WHITE};
int main(void)
{
	SYSCTL->RCGCGPIO |= (1<<5);
	SYSCTL->RCGCTIMER |= 0x7; // 111 -> 0X7 1110 -> 0XE
	delayMs(30);
	
	    GPIOF->LOCK = 0x4C4F434B;   /* unlock commit register */ 
    GPIOF->CR = 0x01;           /* make PORTF0 configurable */ 
    GPIOF->LOCK = 0;            /* lock commit register */ 

	GPIOF->DIR |= WHITE;
	GPIOF->DEN |= WHITE + SWITCHES;
	GPIOF->PUR |= SWITCHES;
	
	    GPIOF->IS  &= ~(1<<4)|~(1<<0);        /* make bit 4, 0 edge sensitive */ 
    GPIOF->IBE &=~(1<<4)|~(1<<0);         /* trigger is controlled by IEV */ 
    GPIOF->IEV &= ~(1<<4)|~(1<<0);        /* falling edge trigger */ 
    GPIOF->ICR |= (1<<4)|(1<<0);          /* clear any prior interrupt */ 
    GPIOF->IM  |= (1<<4)|(1<<0);          /* unmask interrupt */ 
	
	TIMER0->CTL = 0;        		// Disable the timer
	TIMER0->CFG = 0x0;       		// Choose 16-bit mode
	TIMER0->TAMR = 0x02;       		// Periodic mode
	TIMER0->TAILR = 0.5f * 50000000;
	TIMER0->ICR = 0x1;           	// Clear Any Prior Interrupts
	TIMER0->IMR |=(1<<0);			// Enable Timeout Interrupt
	TIMER0->CTL |= 0x01;          	// Enable the timer
	NVIC->ISER[0] = 1<<TIMER0A_IRQn;
	NVIC->ISER[0] = 1<<GPIOF_IRQn;
    while(1)
    {
    }
}

void TIMER0A_Handler()
{
	static int flag = 0;
	if(TIMER0->MIS & 0x1){
		if (flag){
			GPIOF->DATA  = seq[index];
		} else {
			GPIOF->DATA  &= ~WHITE;
		}
		flag = 1 -  flag;
	}
 	TIMER0->ICR = 0x1;
}

void GPIOF_Handler(void) {
	static int flag = 0;
		delayMs(30);
    if (GPIOF->MIS & 0x10) { /* check if interrupt causes by PF4 SW1*/    
				index = (index + 1) %7;
			GPIOF->DATA  = seq[index];
        GPIOF->ICR |= 0x10; /* clear the interrupt flag */ 
    }  
    else if (GPIOF->MIS & 0x01) {  /* check if interrupt causes by PF0/SW2 */ 
					if (flag == 0) {
						TIMER0->TAILR = 0.5f * 50000000;	
					}else {						
						TIMER0->TAILR = 2 * 50000000;	
					}
					flag = 1 - flag;
        GPIOF->ICR |= 0x01; /* clear the interrupt flag */ 
    } 
}

