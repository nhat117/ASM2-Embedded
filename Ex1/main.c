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
    CLK->PWRCON |= (1 << 2);
    while(!(CLK->CLKSTATUS & (1 << 4)));
        
    //Select CPU clock
    //22.11 MHz HXT
    CLK->CLKSEL0 &= (~(0b111<<0));
    CLK->CLKSEL0 |= (1 << 2);
    CLK->PWRCON &= ~(1<<7);// Normal mode
    //Clock frequency divider
    CLK->CLKDIV &= (~(0xF<<0)); 
    CLK->CLKDIV |= 0;
    //System initialization end---------------------
    //GPIO initialization start --------------------
		PB->PMD &= (~(0b11<< 22));
		PB->PMD |= (0b01 << 22); 
    //GPIO initialization end ----------------------
    //Timer 0 initialization start--------------
    //TM0 Clock selection and configuration
    CLK->CLKSEL1 &= ~(0b111 << 20);
    CLK->CLKSEL1 |= (0b000 << 20); // 12Mhz
    CLK->APBCLK |= (1 << 5); // enable timer 3
    //Pre-scale = 240
    TIMER3->TCSR &= ~(0xFF << 0);
    TIMER3->TCSR |= 239;
    //reset Timer 0
    TIMER3->TCSR |= (1 << 26);
    //define Timer 0 operation mode
    TIMER3->TCSR &= ~(0b11 << 27);
    TIMER3->TCSR |= (0b01 << 27);
    TIMER3->TCSR &= ~(1 << 24);
    //TDR to be updated continuously while timer counter is counting
    TIMER3->TCSR |= (1 << 16);
    //Enable TE bit (bit 29) of TCSR
    //The bit will enable the timer interrupt flag TIF
    TIMER3->TCSR |= (1 << 29);
    //TimeOut = 2.4kHz 
    TIMER3->TCMPR = TIMER3_COUNTS;
    //start counting
    TIMER3->TCSR |= (1 << 30);
    //Timer 0 initialization end----------------
    SYS_LockReg();  // Lock protected registers
		
		PC->PMD &= (~(0x03<<24));
		PC->PMD |= (0x01 <<24);
		
    while (1) {
         if (TIMER3->TISR & (1 << 0)) //Wait for the Overflow flag (TIF) to be 
         {
            PC->DOUT ^= (1 << 12); //Toggle
            TIMER3->TISR |= (1 << 0); //Clear the flag by writing 1 to it
         }
					
					//while (!(TIMER3->TDR >= TIMER3_COUNTS));
					//PC->DOUT ^= (1 << 12);
					
      //PC->DOUT ^= (1 << 12);
			//CLK_SysTickDelay(10000);

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