/*Use the onboard LED and another two external LEDs with the TM4C123G board to make one LED flashes every 10 seconds, one flashes every 5 seconds, and one flashes every one second.*/
#include "TM4C123.h"
#define RED 0x02
#define BLUE 0x04
#define GREEN 0x08
#define YELLOW RED + GREEN
#define MAGENTA BLUE + RED
#define CYAN GREEN + BLUE
#define WHITE RED + GREEN + BLUE
#define SW1 0x10
#define SW2 0x01
#define DELAY 900000
#define SRC_CLK 50000000

const int sequence[] = {RED, BLUE, GREEN, YELLOW, MAGENTA, CYAN, WHITE};
int index = 0;
void delay( volatile unsigned long ulLoop ){
    for (ulLoop = 0; ulLoop < DELAY; ulLoop++) {
        for (ulLoop = 0; ulLoop < DELAY; ulLoop++) {
          }
     }
}
int main(void)
{
        SYSCTL->RCGCGPIO |= (1<<5);
        SYSCTL->RCGCTIMER |= (1<<1) | (1<<0) | (1<<2);
        delay(0);
        GPIOF->DIR |= WHITE;
        GPIOF->DEN |= WHITE;
    
    
        TIMER0->CTL = 0;                // Disable the timer
        TIMER0->CFG = 0x0;               // Choose 32-bit mode
        TIMER0->TAMR = 0x02;               // Periodic mode
        TIMER0->TAILR = 1*SRC_CLK - 1;  // Initial Value
        TIMER0->ICR = 0x1;               // Clear Any Prior Interrupts
        TIMER0->IMR |=(1<<0);            // Enable Timeout Interrupt
        TIMER0->CTL |= 0x01;              // Enable the timer
        
        
        TIMER1->CTL = 0;                // Disable the timer
        TIMER1->CFG = 0x0;               // Choose 32-bit mode
        TIMER1->TAMR = 0x02;               // Periodic mode
        TIMER1->TAILR = 5*SRC_CLK - 1;  // Initial Value
        TIMER1->ICR = 0x1;               // Clear Any Prior Interrupts
        TIMER1->IMR |=(1<<0);            // Enable Timeout Interrupt
        TIMER1->CTL |= 0x01;              // Enable the timer
        
        TIMER2->CTL = 0;                // Disable the timer
        TIMER2->CFG = 0x0;               // Choose 32-bit mode
        TIMER2->TAMR = 0x02;               // Periodic mode
        TIMER2->TAILR = 10*SRC_CLK - 1;  // Initial Value
        TIMER2->ICR = 0x1;               // Clear Any Prior Interrupts
        TIMER2->IMR |=(1<<0);            // Enable Timeout Interrupt
        TIMER2->CTL |= 0x01;              // Enable the timer
        
        NVIC->ISER[0] |= (1<<TIMER0A_IRQn);
        NVIC->ISER[0] |= (1<<TIMER1A_IRQn);
        NVIC->ISER[0] |= (1<<TIMER2A_IRQn);
    while(1)
    {
    }
}
void TIMER0A_Handler()
{
    if(TIMER0->MIS & 0x1)
        GPIOF->DATA  ^= BLUE;
     TIMER0->ICR = 0x1;
}
void TIMER1A_Handler()
{
    if(TIMER1->MIS & 0x1)
        GPIOF->DATA  ^= GREEN;
     TIMER1->ICR = 0x1;
}
void TIMER2A_Handler()
{
    if(TIMER2->MIS & 0x1)
        GPIOF->DATA  ^= RED;
     TIMER2->ICR = 0x1;
}

