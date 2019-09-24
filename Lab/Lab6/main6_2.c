#include "stm32l476xx.h"
//These functions inside the asm file
extern void GPIO_init();
extern void max7219_init();
extern void MAX7219Send(unsigned char address, unsigned char data);
//TODO: define your gpio pin
/*#define X0 GPIOXX
#define X1
#define X2
#define X3
#define Y0
#define Y1
#define Y2
#define Y3
unsigned int x_pin = {X0, X1, X2, X3};
unsigned int y_pin = {Y0, Y1, Y2, Y3};*/
/* TODO: initial keypad gpio pin, X as output and Y as input
*/
void keypad_init()
{
	// SET keypad gpio OUTPUT //
	RCC->AHB2ENR = RCC->AHB2ENR|0x2;
	//Set PA8,9,10,12 as output mode
	GPIOA->MODER= GPIOA->MODER&0xFDD5FFFF;
	//set PA8,9,10,12 is Pull-up output
	GPIOA->PUPDR=GPIOA->PUPDR|0x1150000;
	//Set PA8,9,10,12 as medium speed mode
	GPIOA->OSPEEDR=GPIOA->OSPEEDR|0x1150000;
	//Set PA8,9,10,12 as high
	GPIOA->ODR=GPIOA->ODR|10111<<8;
	// SET keypad gpio INPUT //
	//Set PB5,6,7,9 as INPUT mode
	GPIOB->MODER=GPIOB->MODER&0xFFF303FF;
	//set PB5,6,7,9 is Pull-down input
	GPIOB->PUPDR=GPIOB->PUPDR|0x8A800;
	//Set PB5,6,7,9 as medium speed mode
	GPIOB->OSPEEDR=GPIOB->OSPEEDR|0x45400;
}
/* TODO: scan keypad value
* return:
* >=0: key pressed value
* -1: no key press
*/
char keypad_scan()
{
	int flag_keypad, flag_debounce, k, position_c, position_r, flag_keypad_r;
	int Table[4][4] = {{1,2,3,10},{4,5,6,11},{7,8,9,99},{15,0,14,13}};
	while(1){
		flag_keypad=GPIOB->IDR&10111<<5;
		if(flag_keypad!=0){
			k=40000;
			while(k!=0){
				flag_debounce = GPIOB->IDR&10111<<5;
				k--;
			}
			if(flag_debounce!=0){
				for(int i=0;i<4;i++){ //scan keypad from first column
					position_c=i+8;
					if(i==3)position_c++;
					//set PA8,9,10,12(column) low and set pin high from PA8
					GPIOA->ODR=(GPIOA->ODR&0xFFFFE8FF)|1<<position_c;
					for(int j=0;j<4;j++){ //read input from first row
						position_r=j+5;
						if(j==3) position_r++;
						flag_keypad_r=GPIOB->IDR&1<<position_r;
						if(flag_keypad_r!=0){
							display(Table[j][i], 2);
							return Table[j][i];
						}
					}
				}
			}
			GPIOA->ODR=GPIOA->ODR|10111<<8; //set PA8,9,10,12(column) high
		}else {
			display(0, 0);
			return -1;
		}
	}
}
/**
* TODO: Show data on 7-seg via max7219_send
* Input:
* data: decimal value
* num_digs: number of digits will show on 7-seg
* Return:
* 0: success
* -1: illegal data range(out of 8 digits range)
*/
int display(int data, int num_digs)
{
	if (num_digs <= 8 && num_digs >= 0){
		int i;
		int div = 10000000;
		for (i = 8; i > num_digs; i--){
			MAX7219Send(i, 15);
			div = div / 10;
		}
		if (data < 0){
			MAX7219Send(num_digs, 10);
			data = -data;
			num_digs--;
			div = div / 10;
		}
		for (i = num_digs; i > 0; i--){
			int num = data / div;
			MAX7219Send(i, num);
			data = data % div;
			div = div / 10;
		}
		return 0;
	}else
		return -1;
}
int main()
{
	GPIO_init();
	max7219_init();
	keypad_init();
	while(1)
		keypad_scan();
}
