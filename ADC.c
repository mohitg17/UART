// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0
// Last Modified: 11/14/2018
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

// ADC initialization function 
// Input: none
// Output: none
void ADC_Init(void){
 // --UUU-- Complete this(copy from Lab8)
	volatile int delay;
		
	SYSCTL_RCGCGPIO_R |= 0x8;
	while((SYSCTL_PRGPIO_R & 0x8) == 0) {}
	GPIO_PORTD_DIR_R &= ~0x4;
	GPIO_PORTD_AFSEL_R |= 0x4;
	GPIO_PORTD_DEN_R &= ~0x4;
	GPIO_PORTD_AMSEL_R |= 0x4;
	
	SYSCTL_RCGCADC_R |= 0x01;
	delay = SYSCTL_RCGCADC_R;
	delay = SYSCTL_RCGCADC_R;
	delay = SYSCTL_RCGCADC_R;
	delay = SYSCTL_RCGCADC_R;
	ADC0_PC_R = 0x01;
	ADC0_SSPRI_R = 0x0123;
	ADC0_ACTSS_R &= ~0x0008;
	ADC0_EMUX_R &= ~0xF000;
	ADC0_SSMUX3_R = (ADC0_SSMUX3_R & 0xFFFFFFF0)+5;
	ADC0_SSCTL3_R = 0x0006;
	ADC0_IM_R &= ~0x0006;
	ADC0_ACTSS_R |= 0x0008;


}

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t ADC_In(void){  
  uint32_t data;
	ADC0_PSSI_R = 0x8;
	while((ADC0_RIS_R & 0x8)==0){};
		data = ADC0_SSFIFO3_R & 0xFFF;
		ADC0_ISC_R = 0x0008;
		return data;
  // --UUU-- Complete this(copy from Lab8)
}


