#include <stdio.h>
#include "NUC100Series.h"
#define HXT_STATUS 1<<0
#define TIMER3_COUNTS 20

int pressCount = 0;

int main(void)
{
    //System initialization start-------------------
    SYS_UnlockReg(); //Unlock protected regs
    CLK->PWRCON |= (1 << 0); //
    while(!(CLK->CLKSTATUS & (1 << 0)));

        
    //Select CPU clock
    //12 MHz LXT
    CLK->CLKSEL0 &= (~(0b111<<0)); // 12Mhz cpu
	  CLK->CLKSEL0 &= (~(0b111<<3)); //reset stclk_s
    CLK->CLKSEL0 |= (0b011 << 3); // 12Mhz
    CLK->PWRCON &= ~(1<<7);// Normal mode
    //Clock frequency divider
	  //Not using clock divider
    CLK->CLKDIV &= (~(0xF<<0)); 
    //System initialization end---------------------

		
		SysTick->CTRL &= ~(0b111 << 0);
		SysTick->LOAD = 6000000 - 1;
		SysTick->VAL = 0;
		SysTick->CTRL |= (0b011 << 0);
		
    SYS_LockReg();  // Lock protected registers

		
		PC->PMD &= (~(0x03<<24));
		PC->PMD |= (0x01 <<24);

    while (1) {
    }
}

void SysTick_Handler() {
		PC->DOUT ^= (1 << 12);
}

