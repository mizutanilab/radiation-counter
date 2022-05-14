/*********************************************************************
 *
 *                Microchip USB C18 Firmware - Mouse Demo
 *
 *********************************************************************
 * FileName:        user_mouse.c
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC18
 * Compiler:        C18 2.30.01+
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the ìCompanyÅE for its PICmicroÆ Microcontroller is intended and
 * supplied to you, the Companyís customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN ìAS ISÅECONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Rawin Rojvanit       11/19/04    Original.
 ********************************************************************/

/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include <usart.h>
#include "system\typedefs.h"

#include "system\usb\usb.h"

#include "io_cfg.h"             // I/O pin mapping
#include "user\user_mouse.h"

/** V A R I A B L E S ********************************************************/
#pragma udata

char buffer[65];
#define DISP_PERIOD 0x3ff
int idisp = DISP_PERIOD;
//int hcount = 0;
//unsigned char hcourse[60];
unsigned int icount = 0;
unsigned int iminutes = 0;
unsigned int jcount = 0;
unsigned int jtotal = 0;
unsigned char jcourse[60];
unsigned int mcourse[10];
long isigh = 0;
long isigl = 0;
int isw = 0;
int iosc = 0;
int ibattery = 0x7fff;
BOOL bDosemeter = TRUE;
BOOL bBeepOn = TRUE;
char cBattery = 4;
char cDispMode = 0;
BOOL bRE3on = FALSE;

/** P R I V A T E  P R O T O T Y P E S ***************************************/
void delay (unsigned int istep);
void LCDinit(void);
void LCDcursor(int ictrl);
void LCDposition(int iline, int ix);
void LCDprint(char* buf);
void LCDprintHex(unsigned int iout, int idigit);
void LCDprintDec(unsigned int iout, int idigit, int ipoint);
void LCDclear(void);//110626

/** D E C L A R A T I O N S **************************************************/
#pragma code
void UserInit(void)
{
	char msg[] = "Low battery";
	int i, j;
	//reset count
	cDispMode = 4;
	for (i=0; i<60; i++) {jcourse[i] = 0;}
	for (i=0; i<10; i++) {mcourse[i] = 0;}
	//LCD
	LCDinit();
	LCDcursor(0);//cursor off
	LCDclear();//cursor off
	//ADC
	TRISBbits.TRISB0 = 1;
	PIE1bits.ADIE = 0;//disable ADC interrupt
    ADCON1 = 0x02; // ADC input0-12 enabled, voltage reference Vss to Vdd
    ADCON0 = 0x00; // select AN0, AD idle, disable ADC
	//ADCON2 = 0x8e;//right justified, Tacq=2Tad, Tad=64Tosc(1.3us at 48MHz)
	//ADCON2 = 0x95;//right justified, Tacq=4Tad, Tad=16Tosc(0.8us at 20MHz)
	ADCON2 = 0x91;//right justified, Tacq=4Tad, Tad=8Tosc(0.8 us at 10 MHz)
	//ADCON2 = 0x94;//right justified, Tacq=4Tad, Tad=4Tosc(0.8us at 5MHz)
	ADCON0bits.ADON = 1; // enable ADC;
	//LED on
	TRISCbits.TRISC0 = 0;
	TRISCbits.TRISC1 = 0;
	PORTCbits.RC0 = 0;//red LED: off
	PORTCbits.RC1 = 1;//green LED: on
	TRISCbits.TRISC7 = 0;
	PORTCbits.RC7 = 0;//LCD back light: off
	//Interrupt
	INTCONbits.GIE = 0;//Disable interrupts
	RCONbits.IPEN = 1;//enable IPEN (disabled at POR)
	IPR2bits.USBIP = 1;//enable USBIP (same with POR)
	//Timer0: counting period
	//T0CON = 0x08;//stop timer0, 16 bit, internal clk, no prescale
	//T0CON = 0x06;//stop timer0, 16 bit, internal clk, 1/128 ~ 1sec at 48 MHz
	//T0CON = 0x05;//stop timer0, 16 bit, internal clk, 1/64 ~ 1sec at 20 MHz
	//T0CON = 0x04;//stop timer0, 16 bit, internal clk, 1/32 ~ 1sec at 10 MHz
	//T0CON = 0x03;//stop timer0, 16 bit, internal clk, 1/16 ~ 1sec at 5 MHz
	T0CON = 0x05;//stop timer0, 16 bit, internal clk, 1/64, preset 0x676A => 1sec at 10 MHz
	TMR0H = 0x67;
	TMR0L = 0x6a;//preset timer
	INTCONbits.TMR0IE = 1;
	INTCON2bits.TMR0IP = 0;//set timer0 interrupt low priority
	T0CONbits.TMR0ON = 1;//start timer0]
	//Timer1: beep
	//T1CON = 0x30;//stop timer, 8 bit, internal clk, 1/8 prescale
	T1CON = 0x10;//stop timer, 8 bit, internal clk, 1/2 prescale at 10 MHz
	TMR1H = 0x00;
	TMR1L = 0x00;
	PIE1bits.TMR1IE = 1;//enable interrupt
	IPR1bits.TMR1IP = 0;//low priority
	//Comparator
	TRISAbits.TRISA0 = 1;
	TRISAbits.TRISA1 = 1;
	TRISAbits.TRISA2 = 1;
	TRISAbits.TRISA3 = 1;
	CMCON = 0x04;//C2INV=0, C1INV=0, CIS=0, CM2:0=100
	//CMCON = 0x02;//C2INV=0, C1INV=0, CIS=0, CM2:0=010
	IPR2bits.CMIP = 1;//High priority
	PIE2bits.CMIE = 1;//Enable interrupts
	INTCONbits.PEIE = 1;//Enable all peripheral interrupts
	//CCP1 beep
	PR2 = 0xe0;
	CCPR1L = 0x80;
	TRISCbits.TRISC2 = 0;//enable output
	PORTCbits.RC2 = 0;
	//T2CONbits.T2CKPS1 = 1;//prescale 1/16 at 48 MHz
	T2CONbits.T2CKPS1 = 0;//prescale 1/4
	T2CONbits.T2CKPS0 = 1;//prescale 1/4
	T2CONbits.TMR2ON = 1;
	//CCP1CONbits.P1M1 = 0;//PWM single output
	//CCP1CONbits.P1M0 = 0;
	CCP1CONbits.CCP1M3 = 0;//disable CCP1 module in PWM mode
	CCP1CONbits.CCP1M2 = 0;
	CCP1CONbits.CCP1M1 = 0;
	CCP1CONbits.CCP1M0 = 0;
	//correction mode
	if (PORTEbits.RE3 == 0) bDosemeter = FALSE;
	//sense battery voltage
	ADCON0bits.CHS0 = 1;// select AN1
	ADCON0bits.GO_DONE = 1; // start AD ch1
	while (ADCON0bits.GO_DONE) {}
	ibattery = (((int)ADRESH) << 8) | ADRESL;
	ADCON0bits.CHS0 = 0;// back to AN0
	/*
	if (ibattery < 485) {//battery < 7 V
		LCDposition(1, 0);//cursor (line, x)
		LCDprint(msg);
		cDispMode = 5;
	}*/
}//end UserInit

void DispUSBStatus(void)
{
	char msgDetached[] = "Detached  ";
	char msgAttached[] = "Attached  ";
	char msgPowered[] =  "Powered   ";
	char msgDefault[] =  "Default   ";
	char msgAddress[] =  "Address   ";
	char msgConfigured[]="Configured";
        if(usb_device_state == DETACHED_STATE) {
			LCDposition(1, 0);//cursor (line, x)
			LCDprint(msgDetached);
        } else if(usb_device_state == ATTACHED_STATE) {
			LCDposition(1, 0);//cursor (line, x)
			LCDprint(msgAttached);
        } else if(usb_device_state == POWERED_STATE) {
			LCDposition(1, 0);//cursor (line, x)
			LCDprint(msgPowered);
        } else if(usb_device_state == DEFAULT_STATE) {
			LCDposition(1, 0);//cursor (line, x)
			LCDprint(msgDefault);
        } else if(usb_device_state == ADDRESS_STATE) {
			LCDposition(1, 0);//cursor (line, x)
			LCDprint(msgAddress);
        } else if(usb_device_state == CONFIGURED_STATE) {
			LCDposition(1, 0);//cursor (line, x)
			LCDprint(msgConfigured);
        }//end if

}//end DispUSBStatus

void DispSwitch(void) {
	INTCONbits.GIE = 0;//Disable all interrupts
	if (PORTEbits.RE3 == 0) {
		icount = 0;
		LCDclear();
		jcount = 0;
		isigh = 0;
		isigl = 0;
		isw = 1;
	}
	INTCONbits.GIE = 1;//Enable all interrupts
}

void GetSwitch(void) {
	if (PORTEbits.RE3 == 0) {
		if (bRE3on) return;
		bRE3on = TRUE;
		cDispMode++;
	} else {
		if (bRE3on) bRE3on = FALSE;
		return;
	}
	if (cDispMode > 3) cDispMode = 0;
	if ((cDispMode == 0)||(cDispMode == 2)) bBeepOn = TRUE; else bBeepOn = FALSE;
	LCDclear();
}

void UpdateOsc(void) {
	if (isw == 1) iosc++; else return;
	isw = 0;
	if (iosc > 0x13) iosc = 0;
	if (iosc <= 0x0f) {//PRI_RUN mode
		ADCON0bits.ADON = 0; // disable ADC;
		OSCCONbits.SCS1 = 0;
		OSCCONbits.SCS0 = 0;
		ADCON2 = 0x8e;//right justified, Tacq=2Tad, Tad=64Tosc(1.3 us at 48 MHz)
		T0CON = 0x06;//stop timer0, 16 bit, internal clk, 1/128 ~ 1 sec at 48 MHz
		T0CONbits.TMR0ON = 1;//start timer0]
		ADCON0bits.ADON = 1; // enable ADC;
	} else if (iosc == 0x10) {//RC_RUN mode
		ADCON0bits.ADON = 0; // disable ADC;
		OSCCONbits.SCS1 = 1;
		OSCCONbits.SCS0 = 1;
		OSCCONbits.IRCF2 = 1;//8 MHz
		OSCCONbits.IRCF1 = 1;
		OSCCONbits.IRCF0 = 1;
		ADCON2 = 0x91;//right justified, Tacq=4Tad, Tad=8Tosc(1 us at 8 MHz)
		T0CON = 0x04;//stop timer0, 16 bit, internal clk, 1/32 ~ 1 sec at 8 MHz
		T0CONbits.TMR0ON = 1;//start timer0]
		ADCON0bits.ADON = 1; // enable ADC;
	} else if (iosc == 0x11) {//RC_RUN mode
		ADCON0bits.ADON = 0; // disable ADC;
		OSCCONbits.SCS1 = 1;
		OSCCONbits.SCS0 = 1;
		OSCCONbits.IRCF2 = 1;//4 MHz
		OSCCONbits.IRCF1 = 1;
		OSCCONbits.IRCF0 = 0;
		ADCON2 = 0x94;//right justified, Tacq=4Tad, Tad=4Tosc(1 us at 4 MHz)
		T0CON = 0x03;//stop timer0, 16 bit, internal clk, 1/16 ~ 1 sec at 4 MHz
		T0CONbits.TMR0ON = 1;//start timer0]
		ADCON0bits.ADON = 1; // enable ADC;
	} else if (iosc == 0x12) {//RC_RUN mode
		ADCON0bits.ADON = 0; // disable ADC;
		OSCCONbits.SCS1 = 1;
		OSCCONbits.SCS0 = 1;
		OSCCONbits.IRCF2 = 1;//2 MHz
		OSCCONbits.IRCF1 = 0;
		OSCCONbits.IRCF0 = 1;
		ADCON2 = 0x90;//right justified, Tacq=4Tad, Tad=2Tosc(1 us at 2 MHz)
		T0CON = 0x02;//stop timer0, 16 bit, internal clk, 1/8 ~ 1 sec at 2 MHz
		T0CONbits.TMR0ON = 1;//start timer0]
		ADCON0bits.ADON = 1; // enable ADC;
	} else if (iosc >= 0x13) {//RC_RUN mode
		ADCON0bits.ADON = 0; // disable ADC;
		OSCCONbits.SCS1 = 1;
		OSCCONbits.SCS0 = 1;
		OSCCONbits.IRCF2 = 1;//1 MHz
		OSCCONbits.IRCF1 = 0;
		OSCCONbits.IRCF0 = 0;
		ADCON2 = 0x93;//right justified, Tacq=4Tad, Tad=RC(1 us)
		T0CON = 0x01;//stop timer0, 16 bit, internal clk, 1/4 ~ 1 sec at 1 MHz
		T0CONbits.TMR0ON = 1;//start timer0]
		ADCON0bits.ADON = 1; // enable ADC;
	}
}

/******************************************************************************
 * Function:        void ProcessIO(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is a place holder for other user routines.
 *                  It is a mixture of both USB and non-USB tasks.
 *
 * Note:            None
 *****************************************************************************/
void ProcessIO(void)
{   
	char msg[16];
	char status[3];
	unsigned int i, j, k, h;
	int inum, isec;
	long iavg, iavg2;

	//disable interrupt while establishing usb connection
	if ((usb_device_state == CONFIGURED_STATE)||(usb_device_state == POWERED_STATE)) {
		INTCONbits.GIE = 1;//Enable all interrupts
	} else {
		INTCONbits.GIE = 0;//Disable all interrupts
	}

	GetSwitch();
	if (idisp == DISP_PERIOD) {
		if (bDosemeter) {
			GetSwitch();
			//DispUSBStatus();
			//LCDposition(0, 0);//cursor (line, x)
			//LCDprintHex(ibattery, 4);
			if (ibattery < 519) cBattery = 0;
			else if (ibattery < 554) cBattery = 1;
			else if (ibattery < 589) cBattery = 2;
			else if (ibattery < 623) cBattery = 3;
			else cBattery = 4;
			if (bBeepOn) status[0] = 5; else status[0] = 6;//beep 5(on)/6(off)
			status[1] = 8 + cBattery;//battery 12(full)-8(zero)
			status[2] = 0;
			LCDposition(0, 14);//cursor (line, x)
			LCDprint(status);
			j = 0;
			for (i=0; i<60; i++) {j += jcourse[i];}
			h = 0;
			for (i=0; i<10; i++) {h += mcourse[i];}
			if ((cDispMode == 0)||(cDispMode == 1)) {
				if (j >= 10) {
					k = (int)(((long)j) * 898 / 1000);
					LCDposition(0, 0);//cursor (line, x)
					LCDprintDec(k, 5, 1);//data, #digit, position of decimal point
				} else {
					isec = icount % 60;
					if (icount < 600) iavg = icount;
					else iavg = 600 + isec;
					j = 0;
					for (i=0; i<isec; i++) {j += jcourse[i];}
					if (iavg > 0) k = (int)(((long)h + j) * 898 / 1000 * 600 / iavg);
					else k = (int)(((long)h + j) * 898 / 1000);
					LCDposition(0, 0);//cursor (line, x)
					LCDprintDec(k, 5, 2);//data, #digit, position of decimal point
				}
				LCDposition(0, 7);//cursor (line, x)
				msg[0] = 0xe4; msg[1] = 0x53; msg[2] = 0x76; msg[3] = 0x2f; msg[4] = 0x68; msg[5] = 0x00;
				LCDprint(msg);
				LCDposition(1, 0);//cursor (line, x)
				k = (int)(((long)jtotal) * 1497 / 10000);
				LCDprintDec(k, 5, 2);//data, #digit, position of decimal point
				LCDposition(1, 7);//cursor (line, x)
				msg[0] = 0xe4; msg[1] = 0x53; msg[2] = 0x76; msg[3] = 0x00;
				LCDprint(msg);
				LCDposition(1, 11);//cursor (line, x)
				if (icount < 1800) {
					LCDprintDec(icount, 4, 0);//data, #digit, position of decimal point
					msg[0] = 's'; msg[1] = 0;
					LCDprint(msg);
				} else {
					LCDprintDec(iminutes, 4, 0);//data, #digit, position of decimal point
					msg[0] = 'm'; msg[1] = 0;
					LCDprint(msg);
				}
			} else if ((cDispMode == 2)||(cDispMode == 3)) {
				if (j >= 10) {
					LCDposition(0, 0);//cursor (line, x)
					LCDprintDec(j, 5, 0);//data, #digit, position of decimal point
					//LCDposition(0, 6);//cursor (line, x)
				} else {
					isec = icount % 60;
					if (icount < 600) iavg = icount;
					else iavg = 600 + isec;
					j = 0;
					for (i=0; i<isec; i++) {j += jcourse[i];}
					LCDposition(0, 0);//cursor (line, x)
					if (iavg > 0) k = (int)((long)(h + j) * 600 / iavg);
					else k = h + j;
					LCDprintDec(k, 5, 1);//data, #digit, position of decimal point
					//LCDposition(0, 7);//cursor (line, x)
				}
				msg[0] = 0x20; msg[1] = 0x63; msg[2] = 0x70; msg[3] = 0x6d; msg[4] = 0x20; msg[5] = 0x00;
				LCDprint(msg);
				LCDposition(1, 0);//cursor (line, x)
				LCDprintDec(jtotal, 5, 0);//data, #digit, position of decimal point
				LCDposition(1, 6);//cursor (line, x)
				msg[0] = 0x63; msg[1] = 0x6f; msg[2] = 0x75; msg[3] = 0x6e; msg[4] = 0x74; msg[5] = 0x00;
				LCDprint(msg);
				LCDposition(1, 11);//cursor (line, x)
				if (icount < 1800) {
					LCDprintDec(icount, 4, 0);//data, #digit, position of decimal point
					msg[0] = 's'; msg[1] = 0;
					LCDprint(msg);
				} else {
					LCDprintDec(iminutes, 4, 0);//data, #digit, position of decimal point
					msg[0] = 'm'; msg[1] = 0;
					LCDprint(msg);
				}
			} else {
				if (icount >= 4) {
					INTCONbits.GIE = 0;//Disable all interrupts
					cDispMode = 0;
					icount = 0;
					iminutes = 0;
					jcount = 0;
					jtotal = 0;
					for (i=0; i<60; i++) {jcourse[i] = 0;}
					for (i=0; i<10; i++) {mcourse[i] = 0;}
					LCDclear();
					INTCONbits.GIE = 1;//Enable all interrupts
				} else {
					msg[0] = 0x42; msg[1] = 0x6f; msg[2] = 0x6f; msg[3] = 0x74;
					msg[4] = 0x69; msg[5] = 0x6e; msg[6] = 0x67;
					for (i=0; i<icount; i++) {msg[7+i] = 0x2e;}
					msg[7+icount] = 0;
					LCDposition(0, 0);//cursor (line, x)
					LCDprint(msg);
				}
			}//cDispMode
			if (icount & 0x01 == 1) PORTCbits.RC1 = 1; else PORTCbits.RC1 = 0;
		} else {
			DispUSBStatus();
			DispSwitch();
			//UpdateOsc();
	
			LCDposition(0, 7);//cursor (line, x)
			LCDprintHex(icount, 4);
			if (icount & 0x01 == 1) PORTCbits.RC1 = 1; else PORTCbits.RC1 = 0;
			LCDposition(0, 12);//cursor (line, x)
			LCDprintHex(jcount, 4);
			LCDposition(1, 10);//cursor (line, x)
			LCDprintHex(iosc, 4);
			if (jcount > 0) {
				iavg = isigl / jcount;
				iavg2 = isigh / jcount - iavg * iavg;
			} else {
				iavg = 0; iavg2 = 0;
			}
			LCDposition(0, 0);//cursor (line, x)
			LCDprintHex(iavg, 4);
			LCDposition(1, 5);//cursor (line, x)
			LCDprintHex(iavg2, 4);
		}//bCorrection
		idisp = 0;
	}
	idisp++;

	if (usb_device_state != CONFIGURED_STATE) return;//110703

    if (HIDRxReport(buffer, EP1_BUF_SIZE) > 0) // USB receive buffer has data
    {
		if (buffer[0] == 'R') {
			INTCONbits.GIE = 0;//Disable all interrupts
			cDispMode = 0;
			icount = 0;
			iminutes = 0;
			jcount = 0;
			jtotal = 0;
			for (i=0; i<60; i++) {jcourse[i] = 0;}
			for (i=0; i<10; i++) {mcourse[i] = 0;}
			LCDclear();
			INTCONbits.GIE = 1;//Enable all interrupts
		} else if (buffer[0] == 'D') {
			buffer[1] = (icount >> 8) & 0xff;
			buffer[2] = icount & 0xff;
			buffer[3] = (jtotal >> 8) & 0xff;
			buffer[4] = jtotal & 0xff;
			buffer[5] = (ibattery >> 8) & 0xff;
			buffer[6] = ibattery & 0xff;
		}
		//for (i=0; i<16; i++) {
		//	msg[i] = buffer[i] & 0xff;
		//}
		//inum = 0;
		//for (i=0; i<16; i++) {
		//	if ((msg[i] >= 0x30) &&(msg[i] <= 0x39)) {
		//		inum = inum * 10 + (msg[i] - 0x30);
		//	} else {
		//		break;
		//	}
		//}
		//LCDprintHex(inum, 4);
		//
		//buffer[0]+=1;
        while(mHIDTxIsBusy()){}                 // blocking
        HIDTxReport(buffer, EP1_BUF_SIZE);   // transmit packet
	}
    
}//end ProcessIO

/** EOF user_mouse.c *********************************************************/
