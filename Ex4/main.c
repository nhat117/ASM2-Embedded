//------------------------------------------- main.c CODE STARTS 

#include <stdio.h>
#include "NUC100Series.h"
//Macro define when there are keys pressed in each column
#define C3_pressed (!(PA->PIN & (1<<0)))
#define C1_pressed (!(PA->PIN & (1<<2)))
#define LED_W 12
#define LED_E 13
#define LED_WS 14
#define LED_S 15

int tick = 3;
int s1_red = 1;
int main_lane = 1;
int bt_s = 0;
int bt_w = 0;

int pattern[] = {
			 //	 gedbaf_dot_c
					0b10000010,	//Number 0					// ---a----
					0b11101110,	//Number 1					// |			|
					0b00000111,	//Number 2					// f			b
					0b01000110,	//Number 3					// |			|
					0b01101010,	//Number 4					// ---g----
					0b01010010,	//Number 5					// |			|
					0b00010010,	//Number 6					// e			c
					0b11100110,	//Number 7					// |			|
					0b00000010,	//Number 8					// ---d----
					0b01000010,	 //Number 9
					0b11111111	 //Blank LED 
				};		


void set_7seg_we(int num) {
	PC->DOUT |= (1<<7);	 //SC4
	PC->DOUT &= ~(1<<6); //SC3
	PC->DOUT &= ~(1<<5); //SC2
	PC->DOUT &= ~(1<<4); //SC1
	PE->DOUT = pattern[num];
}

void set_7seg_ws(int num) {
	PC->DOUT &= ~(1<<7);//SC4
	PC->DOUT |= (1<<6);  //SC3
	PC->DOUT &= ~(1<<5); //SC2
	PC->DOUT &= ~(1<<4); //SC1
	PE->DOUT = pattern[num];
}

void set_7seg_s(int num) {
	PC->DOUT &= ~(1<<7); //SC4
	PC->DOUT &= ~(1<<6); //SC3
	PC->DOUT |= (1<<5); //SC2
	PC->DOUT &= ~(1<<4); //SC1
	PE->DOUT = pattern[num];
}
								
static void search_col1(void)
{
	// Check Key 1 press
	PA->DOUT &= ~(1<<3);
	PA->DOUT |= (1<<4);
	PA->DOUT |= (1<<5);
	if (C1_pressed)
	{
		bt_w = 1;
		return;
	}
}

static void search_col3(void)
{
	// Check Key 9 press
	PA->DOUT |= (1<<3);
	PA->DOUT |= (1<<4);
	PA->DOUT &= ~(1<<5);
	if (C3_pressed)
	{
		bt_s = 1;
		return;
	}
}

void reset_led() {
	PC->DOUT |= (1 << LED_W);
	PC->DOUT |= (1 << LED_E);
	PC->DOUT |= (1 << LED_WS);
	PC->DOUT |= (1 << LED_S);
}

void switch_we_led() {
	reset_led();
	if (!s1_red) {
		if (PC->PIN & (1 << LED_E)) {
			PC->DOUT &= ~(1 << LED_E);
		}
	} else {
		if (PC->PIN & (1 << LED_W)) {
			PC->DOUT &= ~(1 << LED_W);
		}

	     
		
	}
}

void toggle_we_led() {
	PC->DOUT &= ~(1 << LED_W);
	PC->DOUT |= (1 << LED_E);
	PC->DOUT |= (1 << LED_WS);
	PC->DOUT |= (1 << LED_S);
}

void toggle_ws_led() {
	PC->DOUT |= (1 << LED_W);
	PC->DOUT |= (1 << LED_E);
	PC->DOUT &= ~(1 << LED_WS);
	PC->DOUT |= (1 << LED_S);
}

void toggle_s_led() {
	PC->DOUT |= (1 << LED_W);
	PC->DOUT |= (1 << LED_E);
	PC->DOUT |= (1 << LED_WS);
	PC->DOUT &= ~(1 << LED_S);
}



int main(void)
{		
	SYS_UnlockReg(); // Unlock protected registers
	
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
 
	SYS_LockReg();	// Lock protected registers
		
	// led config, ON -> Green, OFF -> Red
	// led_w
	PC->PMD &= (~(1 << 24));
	PC->PMD |= (1 << 24);
	// led_e
	PC->PMD &= (~(1 << 26));		
	PC->PMD |= (1 << 26);
	// led_ws
	PC->PMD &= (~(1 << 28));
	PC->PMD |= (1 << 28);
	
	//Configure GPIO for Key Matrix
	//Rows - outputs
	PA->PMD &= (~(0b11<< 6));
	PA->PMD |= (0b01 << 6);		
	PA->PMD &= (~(0b11<< 8));
	PA->PMD |= (0b01 << 8);	
	PA->PMD &= (~(0b11<< 10));
	PA->PMD |= (0b01 << 10);	

	//Configure GPIO for 7segment
	//Set mode for PC4 to PC7 
	PC->PMD &= (~(0xFF<< 8)); //clear PMD[15:8] 
	PC->PMD |= (0b01010101 << 8);	 //Set output push-pull for PC4 to PC7
	//Set mode for PE0 to PE7
	PE->PMD &= (~(0xFFFF<< 0)); //clear PMD[15:0] 
	PE->PMD |= 0b0101010101010101<<0;	 //Set output push-pull for PE0 to PE7
	//Select the first digit U11
	
	PA->DBEN |= (0b111111 << 0);
	GPIO->DBNCECON &= ~(1 << 4);
	GPIO->DBNCECON &= ~(0xF << 0);
	GPIO->DBNCECON |= 0xC << 0;
	
	while(1)
	{
		//Turn all Rows to LOW 
		PA->DOUT &= ~(1<<3);
		PA->DOUT &= ~(1<<5);
		if(C1_pressed) 
		{
			search_col1();
		}
		else if(C3_pressed) 
		{
			search_col3();
		}
		else {
			// Do nothing
		}
		
		if (main_lane) {
			if (tick == 3) {
				switch_we_led();
				set_7seg_we(3);
			}
			else if (tick == 2) {
				//switch_we_led();
				set_7seg_we(2);
			}
			else if (tick == 1) {
				//switch_we_led();
				set_7seg_we(1);
				tick = 4;
				s1_red = s1_red == 0 ? 1 : 0;
				if (bt_w) {
					main_lane = 0;
					tick = 3;
				} else if (bt_s) {
					main_lane = 0;
					tick = 3;
				}
			}		
		} else if (bt_w) {
			if (tick == 2) {
				toggle_ws_led();
				set_7seg_ws(2);
			} else if (tick == 1) {
				set_7seg_ws(1);
				if (bt_s) {
					tick = 3;
					bt_w = 0;
				} else {
					main_lane = 1;
					bt_w = 0;
					tick = 4;
				}
			}
		} else if (bt_s) {
			if (tick == 2) {
				toggle_s_led();
				set_7seg_s(2);
			} else if (tick == 1) {
				set_7seg_s(1);
				main_lane = 1;
				bt_s = 0;
				tick = 4;
			}
		}	
	}
}
//------------------------------------------- main.c CODE ENDS 


void SysTick_Handler() {
	--tick;
}
