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
/* TODO: scan keypad value and display
*/
void keypad_scan()
{
	int flag_keypad, flag_debounce, k, position_c, position_r, flag_keypad_r;
	int Table[4][4] = {{1,2,3,10},{4,5,6,11},{7,8,9,12},{15,0,14,13}};
	int currentState[16], lastState[16], change[16];
	int sum = 0;
	for (int i = 0; i < 16; i++)
		currentState[i] = lastState[i] = change[i] = 0;
	while(1){
		for (int i = 0; i < 16; i++)
			change[i] = 0;
		for(int i=0;i<4;i++){ //scan keypad from first column
			position_c=i+8;
			if(i==3)position_c++;
			//set PA8,9,10,12(column) low and set pin high from PA8
			GPIOA->ODR=(GPIOA->ODR&0xFFFFE8FF)|1<<position_c;
			for(int j=0;j<4;j++){ //read input from first row
				position_r=j+5;
				if(j==3) position_r++;
				flag_keypad_r=GPIOB->IDR&1<<position_r;
				flag_keypad_r = flag_keypad_r && 1;
				if (flag_keypad_r != currentState[Table[j][i]])
					change[Table[j][i]] = 1;
				if (flag_keypad_r != 0 && (Table[j][i] == 15 || Table[j][i] == 14))
					sum = 0;
			}
		}
		int flag = 0;
		for (int i = 0; i < 16; i++){
			if (change[i] && i != 15 && i != 14)
				flag = 1;
		}
		if (flag){
			k=100;
			while(k!=0){
				flag_debounce = GPIOB->IDR&10111<<5;
				k--;
			}
			for(int i=0;i<4;i++){ //scan keypad from first column
				position_c=i+8;
				if(i==3)position_c++;
				//set PA8,9,10,12(column) low and set pin high from PA8
				GPIOA->ODR=(GPIOA->ODR&0xFFFFE8FF)|1<<position_c;
				for(int j=0;j<4;j++){ //read input from first row
					position_r=j+5;
					if(j==3) position_r++;
					flag_keypad_r=GPIOB->IDR&1<<position_r;
					flag_keypad_r = flag_keypad_r && 1;
					if (change[Table[j][i]]){
						if(flag_keypad_r != currentState[Table[j][i]]){
							currentState[Table[j][i]] = !currentState[Table[j][i]];
						}
						if (currentState[Table[j][i]]){
							sum += Table[j][i];
							if(sum>9999999)
							{
								sum = 9999999;
							}
						}

					}
				}
			}
		}
		/*for(int i=0;i<4;i++){ //scan keypad from first column
			position_c=i+8;
			if(i==3)position_c++;
			//set PA8,9,10,12(column) low and set pin high from PA8
			GPIOA->ODR=(GPIOA->ODR&0xFFFFE8FF)|1<<position_c;
			for(int j=0;j<4;j++){ //read input from first row
				position_r=j+5;
				if(j==3) position_r++;
				flag_keypad_r=GPIOB->IDR&1<<position_r;
				flag_keypad_r = flag_keypad_r && 1;
				lastState[Table[j][i]] = flag_keypad_r;
			}
		}*/
		display(sum, 8);
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
	keypad_scan();
}
