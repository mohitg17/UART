// Uart.c
// Runs on LM4F120/TM4C123
// Use UART1 to implement bidirectional data transfer to and from 
// another microcontroller in Lab 9.  This time, interrupts and FIFOs
// are used.
// Daniel Valvano
// November 14, 2018
// Modified by EE345L students Charlie Gough && Matt Hawk
// Modified by EE345M students Agustinus Darmawan && Mingjie Qiu

/* Lab solution, Do not post
 http://users.ece.utexas.edu/~valvano/
*/

// This U0Rx PC4 (in) is connected to other LaunchPad PC5 (out)
// This U0Tx PC5 (out) is connected to other LaunchPad PC4 (in)
// This ground is connected to other LaunchPad ground

#include <stdint.h>
#include "Fifo.h"
#include "Uart.h"
#include "../inc/tm4c123gh6pm.h"
#define PF2     (*((volatile uint32_t *)0x40025010))
#define UART_CTL_TXE 0x00000100 // UART Transmit FIFO enable
#define UART_CTL_RXE 0x00000200 // UART Receive FIFO enable

int RxCounter = 0;
uint32_t UART_Error = 0; // Overflow counter

void EnableInterrupts(void);  // Enable interrupts
uint32_t DataLost; 
// Initialize UART1
// Baud rate is 115200 bits/sec
// Make sure to turn ON UART1 Receiver Interrupt (Interrupt 6 in NVIC)
// Write UART1_Handler
void Uart_Init(void){
   // --UUU-- complete with your code
	volatile int delay;						// UART Delay
	SYSCTL_RCGCUART_R |= 0x0002;
	delay = SYSCTL_RCGCUART_R;
	SYSCTL_RCGCGPIO_R |= 0x04;
	delay = SYSCTL_RCGCUART_R;
	Fifo_Init();											// call Fifo_Init
	UART1_CTL_R &= ~0x0001;					  // disable UART
	UART1_IBRD_R = 43;							 	// baud rate: 80MHz / (16* 115200)
	UART1_FBRD_R = 26;							 	// fractional part: round(0.40278 * 64) = 26
	UART1_IFLS_R  =0;
	UART1_IFLS_R |= UART_IFLS_RX4_8; 	// Fifo Interrupt at 1/2 full
	UART1_IM_R |= 0x10;								// arm interrupts
	UART1_LCRH_R = 0x0070;						// 8 bit length, enable FIFO
	UART1_CTL_R |= 0x0301;					// enable UART
	//UART1_CTL_R |= (UART_CTL_UARTEN|UART_CTL_TXE|UART_CTL_RXE); // enable UART
	
	GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R & 0xFF00FFFF)+0x00220000;
	GPIO_PORTC_AMSEL_R &= ~0x30;			// turn off analog
	GPIO_PORTC_AFSEL_R |= 0x30;				//enable alternate function
	GPIO_PORTC_DEN_R |= 0x30;
	//UART1_IFLS_R = (UART1_IFLS_R& (~0x38)) +0x10;
	NVIC_PRI1_R = (NVIC_PRI1_R & 0xFF0FFFFF)+0x00600000;
	NVIC_EN0_R |= 0x40; //NVIC_EN0_INT6;		// Enable level 6 interrupt

}

// input ASCII character from UART
// spin if RxFifo is empty
// Receiver is interrupt driven
char Uart_InChar(void){
  while ((UART1_FR_R & 0x0010) != 0) {}			// wait if RxFIFO is empty
	return ((unsigned char)(UART1_DR_R&0xFF));// --UUU-- remove this, replace with real code
}

//------------UART1_InMessage------------
// Accepts ASCII characters from the serial port
//    and adds them to a string until ETX is typed
//    or until max length of the string is reached.
// Input: pointer to empty buffer of 8 characters
// Output: Null terminated string
// THIS FUNCTION IS OPTIONAL
void UART1_InMessage(char *bufPt){
}

//------------UART1_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
// Transmitter is busywait
void Uart_OutChar(char data){
  // --UUU-- complete with your code
	 while ((UART1_FR_R & 0x0020)!= 0); // wait 

	UART1_DR_R = data;
}

// hardware RX FIFO goes from 7 to 8 or more items
// UART receiver Interrupt is triggered; This is the ISR
void UART1_Handler(void){
  // --UUU-- complete with your code
	char letters;
  PF2 ^= 0x4;
	PF2 ^= 0x4;
	while((UART1_FR_R & UART_FR_RXFE) == 0 )
		{
		letters = UART1_DR_R;					// Gets value from UART Fifo
		UART_Error += Fifo_Put(letters); // Counts times the Fifo is full
		}
	RxCounter++;											// Counts num times data received
	UART1_ICR_R = 0x10;								// UART Handler complete
	
	PF2 ^= 0x4;
}
