#include <stdio.h> 
#include "NUC100Series.h" 
 
#define HXT_STATUS 1<<0 
#define PLL_STATUS 1<<2 
#define PLLCON_FB_DV_VAL 10 
#define CPUCLOCKDIVIDE 1 


//Define c code
int main (void) {
	SYS_UnlockReg(); //Unlock protected register
	
	
	//Enable clock source
	CLK->PWRCON |= 1<<0;
	
	while(!(CLK->CLKSTATUS & HXT_STATUS));
	
	//PLL configuration starts
	CLK->PLLCON &=(~(1<<19));
	CLK->PLLCON &=(~(1<<16));
	CLK->PLLCON &=(~(0x01FF<<0));
	CLK->PLLCON |= PLLCON_FB_DV_VAL;
	CLK->PLLCON &= (~(1<<18));
	while(!(CLK->CLKSTATUS & PLL_STATUS));
	
	
	//Clock source selection
	CLK->CLKSEL0 &= (~(0x07<<0));
	CLK->CLKSEL0 |= 0x02;
	
	//Clock frequency division
	CLK->CLKDIV &= 0x0F;
	CLK->CLKDIV |= (CPUCLOCKDIVIDE-1);
	
	//Lock protected register
	SYS_LockReg();
	
	//Pin mode configuration
	PC->PMD &= (~(0x03<<24));
	PC->PMD |= (0x01 <<24);
	
	while(1) {
		PC->DOUT ^= (1<<12);
	}
}