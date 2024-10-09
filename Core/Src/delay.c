#define CPU_FREQUENCY_MHZ (int)(HAL_RCC_GetHCLKFreq()/1000000)
#include "main.h"
void delay_us( uint32_t delay) {
	int last, curr, val;
	int temp;

	while (delay != 0) {
		temp = delay > 900 ? 900 : delay;
		last = SysTick->VAL;
		curr = last - CPU_FREQUENCY_MHZ * temp;
		if (curr >= 0) {
			do {
				val = SysTick->VAL;
			} while ((val < last) && (val >= curr));
		} else {
			curr += CPU_FREQUENCY_MHZ * 1000;
			do {
				val = SysTick->VAL;
			} while ((val <= last) || (val > curr));
		}
		delay -= temp;
	}
}
