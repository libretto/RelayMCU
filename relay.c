/*

    Code for STC15F104W MCU which was installed in cheap ESP-01 relay module marked "LCTECH".
    I bought few such modules from ebay but they had broken MCU firmware and not works. 
    UART interaction code I got from STC15F100 docs
    http://www.stcmicro.com/datasheet/STC15F100-en.pdf

    To compile this code under linux run: 

    sdcc -mmcs51 --iram-size 128 --xram-size 0 --code-size 4096  --nooverlay --noinduction --verbose --debug -V --std-sdcc89 --model-small   "relay.c"

    To upload code into 

    stcgal -p /dev/ttyUSB1 -b 1200 -D -t 11059 relay.ihx

*/

#include <8051.h>

#define relay_pin P3_2

    
__sfr __at(0x8E) AUXR;
__sfr __at(0xB1) P3M1;
__sfr __at(0xB2) P3M0;

#define RXB P3_0
#define TXB P3_1
#define BAUD 0xFE80

typedef __bit BOOL;
typedef unsigned char BYTE;
typedef unsigned int WORD;
BYTE TBUF, RBUF;
BYTE TDAT, RDAT;
BYTE TCNT, RCNT;
BYTE TBIT, RBIT;
BOOL TING, RING;
BOOL TEND, REND;
BYTE t, r;
BYTE buf[16];

BYTE step;
BYTE chr;
BYTE mode;
BOOL OK;
BOOL ANSWER;
BYTE answer_data;
int i;

void uart_send(char chr);
void uart_init();

void main()
{
	char a[] = { 'U','P' };

// set push-pull mode to P3.2 

    P3M1=0;
    P3M0=4;

	relay_pin = 0;

	step = 0;
	mode = 0;
	OK = 0;


	uart_init();

	for (i = 0; a[i] != 0; i++)
		uart_send(a[i]);     

	while (1) {              
		if (REND) {
			REND = 0;
			chr = RBUF;
			switch (step) {
			case 0:
				if (chr == 0xA0)
					step++;

				break;
			case 1:
				if (chr == 0x01)
					step++;
				else
					step = 0;
				break;
			case 2:
				if (chr == 0x01 || chr == 0x00) {
					step++;
					mode = chr;
				} else {
					step = 0;
				}
				break;
			case 3:
				if (chr == 0xA2 && mode == 0x01) {
					relay_pin = 1;
					OK = 1;
				} else {
					if (chr == 0xA1 && mode == 0) {
						relay_pin = 0;
						OK = 1;
					}
				}
				step = 0;
				break;
			}
		}

		if (OK) {
			OK = 0;
			uart_send(relay_pin);
		}
	}
}


void tm0(void) __interrupt 1 __using 1
{
	if (RING) {
		if (--RCNT == 0) {
			RCNT = 3; 
			if (--RBIT == 0) {
				RBUF = RDAT;
				RING = 0;   
				REND = 1;   
			} else {
				RDAT >>= 1;
				if (RXB) RDAT |= 0x80; 
			}
		}
	} else if (!RXB) {
		RING = 1;     
		RCNT = 4;     
		RBIT = 9;     
	}
	if (--TCNT == 0) {
		TCNT = 3;     
		if (TING) {   
			if (TBIT == 0) {
				TXB = 0; 
				TDAT = TBUF;
				TBIT = 9;   
			} else {
				TDAT >>= 1; 
				if (--TBIT == 0) {
					TXB = 1;
					TING = 0;
					TEND = 1;
				} else {
					TXB = CY;
				}
			}
		}
	}
}

void uart_send(char chr)
{
	while (!TEND) ;
	TEND = 0;
	TBUF = chr;
	TING = 1;
}


void uart_init()
{
	SCON = 0x50; // Asynchronous mode, 8-bit data and 1-stop bit
	EA = 0;
	PT0 = 0;
	ET0 = 0;                //enable timer0 interrupt
	TR0 = 0;                //tiemr0 start running
	TMOD = 0x00;            //timer0 in 16-bit auto reload mode
	AUXR = 0x80;            //timer0 working at 1T mode
	TL0 = BAUD;
	TH0 = BAUD >> 8;        //initial timer0 and set reload value
	TR0 = 1;                //tiemr0 start running
	ET0 = 1;                //enable timer0 interrupt
	PT0 = 1;                //improve timer0 interrupt priority
	EA = 1;                 //open global interrupt switch


	TING = 0;
	RING = 0;
	TEND = 1;
	REND = 0;
	TCNT = 0;
	RCNT = 0;
}




