#include <stdio.h>
#include "NUC100Series.h"
#define HXT_STATUS 1<<0
#define TIMER3_COUNTS 20
int main(void)
{
    //System initialization start-------------------
    SYS_UnlockReg(); //Unlock protected regs
    CLK->PWRCON |= (1 << 0); //
    while(!(CLK->CLKSTATUS & (1 << 0)));
    //CLK->PWRCON |= (1 << 2);
    //while(!(CLK->CLKSTATUS & (1 << 4)));
        
    //Select CPU clock
    //12 MHz LXT
    CLK->CLKSEL0 &= (~(0b111<<0));
    CLK->CLKSEL0 |= (0b000 << 0); // 12Mhz
    CLK->PWRCON &= ~(1<<7);// Normal mode
    //Clock frequency divider
	  //Not using clock divider
    CLK->CLKDIV &= (~(0xF<<0)); 
    CLK->CLKDIV |= 0;
    //System initialization end---------------------

		
    //TM0 Clock selection and configuration
    CLK->CLKSEL1 &= ~(0b111 << 8);
    CLK->CLKSEL1 |= (0b010 << 8); // 12Mhz
		CLK->CLKSEL1 &= ~(0b111 << 12);
		CLK->CLKSEL1 |= (0b010 << 12);
    CLK->APBCLK |= (1 << 2); // enable timer 0
		CLK->APBCLK |= (1 << 3); // enable timer 1 
		
		// Timer 0 config
    //Pre-scale = 12
    TIMER0->TCSR &= ~(0xFF << 0);
    TIMER0->TCSR |= 12 - 1;
    //reset Timer 0
    TIMER0->TCSR |= (1 << 26);
    //define Timer 0 operation mode
    TIMER0->TCSR &= ~(0b11 << 27);
    TIMER0->TCSR |= (0b01 << 27);
    TIMER0->TCSR &= ~(1 << 24);
    //TDR to be updated continuously while timer counter is counting
    TIMER0->TCSR |= (1 << 16);
    //Enable TE bit (bit 29) of TCSR
    //The bit will enable the timer interrupt flag TIF
    TIMER0->TCSR |= (1 << 29);
    
    TIMER0->TCMPR = 499999;
    //start counting
    TIMER0->TCSR |= (1 << 30);
		
		////////////////////////////////////////////////
		//Timer 1 config
		TIMER1->TCSR &= ~(0xFF << 0);
    TIMER1->TCSR |= 12 - 1;
    //reset Timer 1
    TIMER1->TCSR |= (1 << 26);
    //define Timer 0 operation mode
    TIMER1->TCSR &= ~(0b11 << 27);
    TIMER1->TCSR |= (0b01 << 27);
    TIMER1->TCSR &= ~(1 << 24);
    //TDR to be updated continuously while timer counter is counting
    TIMER1->TCSR |= (1 << 16);
    //Enable TE bit (bit 29) of TCSR
    //The bit will enable the timer interrupt flag TIF
    TIMER1->TCSR |= (1 << 29);
    
    TIMER1->TCMPR = 199;
    //start counting
    TIMER1->TCSR |= (1 << 30);

    SYS_LockReg();  // Lock protected registers
		
		PC->PMD &= (~(0x03<<24));
		PC->PMD |= (0x01 <<24);
		PC->PMD &= (~(0x03<<26));
		PC->PMD |= (0x01 <<26);
		
    while (1) {
         if (TIMER0->TISR & (1 << 0)) //Wait for the Overflow flag (TIF) to be 
         {
            PC->DOUT ^= (1 << 13); //Toggle
            TIMER0->TISR |= (1 << 0); //Clear the flag by writing 1 to it
         }
				 
				 if (TIMER1->TISR & (1 << 0)) //Wait for the Overflow flag (TIF) to be 
         {
            PC->DOUT ^= (1 << 12); //Toggle
            TIMER1->TISR |= (1 << 0); //Clear the flag by writing 1 to it
         }
				

    }
}
// //------------------------------------------- main.c CODE ENDS 

//------------------------------------------- main.c CODE STARTS -------------
// #include <stdio.h>
// #include "NUC100Series.h"
// #define HXT_STATUS 1<<0
// #define TIMER0_COUNTS 800000-1
// int main(void)
// {
//     //System initialization start-------------------
//     SYS_UnlockReg(); //Unlock protected regs
//     CLK->PWRCON |= (1 << 0);
//         while(!(CLK->CLKSTATUS & HXT_STATUS));
//     //Select CPU clock
//     //12 MHz HXT
//     CLK->CLKSEL0 &= (~(0b111<<0));
//     CLK->PWRCON &= ~(1<<7);// Normal mode
//     //Clock frequency divider
//     CLK->CLKDIV &= (~(0xF<<0)); 
//     //System initialization end---------------------
//     //GPIO initialization start --------------------
//     //GPIOC.14: output push-pull
//         PC->PMD &= (~(0b11<< 28));
//         PC->PMD |= (0b01 << 28);    
//     //GPIO initialization end ----------------------
//     //Timer 0 initialization start--------------
//     //TM0 Clock selection and configuration
//     CLK->CLKSEL1 &= ~(0b111 << 8);
//     CLK->CLKSEL1 |= (0b010 << 8);
//     CLK->APBCLK |= (1 << 2);
//     //Pre-scale =11
//     TIMER0->TCSR &= ~(0xFF << 0);
//     TIMER0->TCSR |= 11 << 0;
//     //reset Timer 0
//     TIMER0->TCSR |= (1 << 26);
//     //define Timer 0 operation mode
//     TIMER0->TCSR &= ~(0b11 << 27);
//     TIMER0->TCSR |= (0b01 << 27);
//     TIMER0->TCSR &= ~(1 << 24);
//     //TDR to be updated continuously while timer counter is counting
//     TIMER0->TCSR |= (1 << 16);
//     //Enable TE bit (bit 29) of TCSR
//     //The bit will enable the timer interrupt flag TIF
//     TIMER0->TCSR |= (1 << 29);
//     //TimeOut = 0.8s --> Counter's TCMPR = 800000-1
//     TIMER0->TCMPR = TIMER0_COUNTS;
//     //start counting
//     TIMER0->TCSR |= (1 << 30);
//     //Timer 0 initialization end----------------
//     SYS_LockReg();  // Lock protected registers
//     while (1) {
//         if (TIMER0->TISR & (1 << 0)) //Wait for the Overflow flag (TIF) to be 

//     {
//         PC->DOUT ^= 1 << 14; //Toggle
//         TIMER0->TISR |= (1 << 0); //Clear the flag by writing 1 to it
//     }
//     }
// }
// //------------------------------------------- main.c CODE ENDS