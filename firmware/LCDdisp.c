// Compile options:  -ml (Large code model) 

#include <string.h>
#include <stdio.h>
#if defined(__18F2550)
	#include <p18F2550.h>
#elif defined(__18F14K50)
	#include <p18F14K50.h>
#endif

void delay (unsigned int istep) {
	unsigned int i;
	for (i=istep; i>0; i--) {}
}

void LCDoutput(int ictrl, int ibyte) {
	//0:mode, 1:data
	if (ictrl) {
		if (LATB & 0x04) ; else PORTB = LATB | 0x04; 
	} else {
		if (LATB & 0x04) PORTB = LATB & 0xfb; ;
	}
	//command / data
	PORTB = LATB | 0x08;
	PORTB = (LATB & 0x0f) | (ibyte & 0xf0);
	delay(10);
	PORTB = LATB & 0xf7;
	delay(10);
	PORTB = LATB | 0x08;
	//lower 4 bits
	PORTB = (LATB & 0x0f) | ((ibyte << 4) & 0xf0);
	delay(10);
	PORTB = LATB & 0xf7;
	delay(10);
	PORTB = LATB | 0x08;
	//wait
	if ((ictrl == 0)&&(ibyte == 0x01)) delay(3000);
	else if ((ictrl == 0)&&(ibyte == 0x02)) delay(3000);
	else if ((ictrl == 0)&&(ibyte == 0x03)) delay(3000);
}
void LCDcursor(int ictrl) {
	if (ictrl) LCDoutput(0, 0x0f);
	else LCDoutput(0, 0x0c);
}
void LCDposition(int iline, int ix) {
	int ipos;
	if (iline & 0x01) ipos = 0xc0;
	else ipos = 0x80;
	ipos = ipos | (ix & 0x0f);
	LCDoutput(0, ipos);
}
void LCDprint(char* buf) {
	int i;
	const int isize = strlen(buf);
	for (i=0; i<isize; i++) {
		LCDoutput(1, buf[i]);
	}
}
void LCDprintHex(unsigned int iout, int idigit) {
	char buf;
	int i;
	for (i=3; i>=0; i--) {
		buf = (iout >> (i * 4)) & 0x000f;
		if (buf < 10) buf += 0x30; else buf += 0x37;
		if (i < idigit) LCDoutput(1, buf);
	}
}

void LCDprintDec(unsigned int iout, int idigit, int ipoint) {
	char buf[20];
	int i, ipos, ichar;
	ipos = 16 - idigit;
	ichar = idigit;
	if (ipoint > 0) ichar++;
	else if (ipoint < 0) ipoint = 0;
	sprintf (buf, "%16u", iout);
	for (i=0; i<ichar; i++) {
		if (i == idigit - ipoint) {
			LCDoutput(1, '.');
			continue;
		} else if (i >= idigit - ipoint - 1) {
			if (buf[ipos] == ' ') buf[ipos] = '0';
		}
		LCDoutput(1, buf[ipos]);
		ipos++;
	}
}

void LCDclear(void) {
	LCDoutput(0, 0x01);
}
void LCDinit(void) {
	int i;
	//init LCD port
	TRISBbits.TRISB7 = 0;
	TRISBbits.TRISB6 = 0;
	TRISBbits.TRISB5 = 0;
	TRISBbits.TRISB4 = 0;
	TRISBbits.TRISB3 = 0;
	TRISBbits.TRISB2 = 0;
	//RS = 0, STB = 1
	PORTB = LATB & 0xfb;//RB2=0
	PORTB = LATB | 0x08;//RB3=1
	delay(30000);
	//////
	PORTB = LATB & 0x0f;
	PORTB = LATB | 0x30;//RB7-4=0011
	delay(1000);
	PORTB = LATB & 0xf7;//RB3=0
	delay(10000);
	PORTB = LATB | 0x08;//RB3=1
	delay(1000);
	PORTB = LATB & 0xf7;//RB3=0
	delay(10000);
	PORTB = LATB | 0x08;//RB3=1
	delay(1000);
	PORTB = LATB & 0xf7;//RB3=0
	delay(10000);
	PORTB = LATB | 0x08;//RB3=1
	//////
	//4 bit mode
	PORTB = LATB & 0x0f;
	PORTB = LATB | 0x20;//RB7-4=0010
	delay(10);
	PORTB = LATB & 0xf7;//RB3=0
	delay(10);
	PORTB = LATB | 0x08;//RB3=1
	//4 bit mode, 2 lines, 5x7 dots
	LCDoutput(0, 0x28);
	//increment mode, shift off
	LCDoutput(0, 0x06);
	//display on, cursor on, blink on
	LCDcursor(1);
	//
	LCDoutput(0, 64);//110711 CGRAM address 0
	LCDoutput(1, 0B01110);//0
	LCDoutput(1, 0B11011);//1
	LCDoutput(1, 0B10001);//2
	LCDoutput(1, 0B10001);//3
	LCDoutput(1, 0B10001);//4
	LCDoutput(1, 0B10001);//5
	LCDoutput(1, 0B11111);//6
	LCDoutput(1, 0B00000);//7
	//
	LCDoutput(1, 0B01110);//0
	LCDoutput(1, 0B11011);//1
	LCDoutput(1, 0B10001);//2
	LCDoutput(1, 0B10001);//3
	LCDoutput(1, 0B10001);//4
	LCDoutput(1, 0B11111);//5
	LCDoutput(1, 0B11111);//6
	LCDoutput(1, 0B00000);//7
	//
	LCDoutput(1, 0B01110);//0
	LCDoutput(1, 0B11011);//1
	LCDoutput(1, 0B10001);//2
	LCDoutput(1, 0B10001);//3
	LCDoutput(1, 0B11111);//4
	LCDoutput(1, 0B11111);//5
	LCDoutput(1, 0B11111);//6
	LCDoutput(1, 0B00000);//7
	//
	LCDoutput(1, 0B01110);//0
	LCDoutput(1, 0B11011);//1
	LCDoutput(1, 0B10001);//2
	LCDoutput(1, 0B11111);//3
	LCDoutput(1, 0B11111);//4
	LCDoutput(1, 0B11111);//5
	LCDoutput(1, 0B11111);//6
	LCDoutput(1, 0B00000);//7
	//
	LCDoutput(1, 0B01110);//0
	LCDoutput(1, 0B11111);//1
	LCDoutput(1, 0B11111);//2
	LCDoutput(1, 0B11111);//3
	LCDoutput(1, 0B11111);//4
	LCDoutput(1, 0B11111);//5
	LCDoutput(1, 0B11111);//6
	LCDoutput(1, 0B00000);//7
	//
	LCDoutput(1, 0B00001);//0
	LCDoutput(1, 0B00011);//1
	LCDoutput(1, 0B11101);//2
	LCDoutput(1, 0B10101);//3
	LCDoutput(1, 0B11101);//4
	LCDoutput(1, 0B00011);//5
	LCDoutput(1, 0B00001);//6
	LCDoutput(1, 0B00000);//7
	//
	LCDoutput(1, 0B10100);//7
	LCDoutput(1, 0B01000);//7
	LCDoutput(1, 0B10101);//0
	LCDoutput(1, 0B00011);//1
	LCDoutput(1, 0B11101);//2
	LCDoutput(1, 0B11101);//4
	LCDoutput(1, 0B00011);//5
	LCDoutput(1, 0B00001);//6
	//
	LCDoutput(0, 0x02);//return home
}
