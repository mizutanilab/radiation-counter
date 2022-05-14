/*********************************************************************
 *
 *                Microchip USB C18 Firmware Version 1.0
 *
 *********************************************************************
 * FileName:        main.c
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
#include "system\typedefs.h"                        // Required
#include "system\usb\usb.h"                         // Required
#include "io_cfg.h"                                 // Required

#include "system\usb\usb_compile_time_validation.h" // Optional
#include "user\user_mouse.h"                        // Modifiable

/** V A R I A B L E S ********************************************************/
#pragma udata

/** P R I V A T E  P R O T O T Y P E S ***************************************/
static void InitializeSystem(void);
void USBTasks(void);

void YourHighPriorityISRCode();
void YourLowPriorityISRCode();
extern unsigned int icount;
extern unsigned int iminutes;
extern unsigned int jcount;
extern unsigned char jcourse[60];
extern unsigned int mcourse[60];
extern unsigned int jtotal;
extern long isigh;
extern long isigl;
extern BOOL bDosemeter;
extern int ibattery;
extern BOOL bBeepOn;

/** V E C T O R  R E M A P P I N G *******************************************/
//110626
//extern void _startup (void);        // See c018i.c in your C18 compiler dir
//#pragma code _RESET_INTERRUPT_VECTOR = 0x000800
//void _reset (void)
//{
//    _asm goto _startup _endasm
//}
/** VECTOR REMAPPING ***********************************************/
#if defined(__18CXX)
	#define REMAPPED_RESET_VECTOR_ADDRESS			0x00
	#define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS	0x08
	#define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS	0x18
	
	#pragma code REMAPPED_HIGH_INTERRUPT_VECTOR = REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS
	void Remapped_High_ISR (void)
	{
		//PIR2bits.CMIF = 0;
		//if (CMCONbits.C1OUT) return;//detect rising edge (C1- > C1+)
	    //ADCON0bits.GO_DONE = 1; // start AD ch0
		_asm
		bcf		PIR2,6,0
		btfsc	CMCON,6,0
		retfie	1
		bsf		ADCON0,1,0
		goto YourHighPriorityISRCode
		_endasm
	}
	#pragma code REMAPPED_LOW_INTERRUPT_VECTOR = REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS
	void Remapped_Low_ISR (void)
	{
	     _asm goto YourLowPriorityISRCode _endasm
	}
	
	#pragma code
	
	#pragma tmpdata ISRtmpdata	// use ISRtmpdata
	//These are your actual interrupt handling routines.
	#pragma interrupt YourHighPriorityISRCode nosave=section(".tmpdata"), TBLPTRL, TBLPTRH, TBLPTRU, TABLAT, PCLATH, PCLATU
	void YourHighPriorityISRCode()
	{
		int iadc;

		//if (CMCONbits.C1OUT == 1) {//detect falling edge (C1- < C1+)
		//this setting gives many noisy counts which would come from some comparator misconfiguration
		//if (CMCONbits.C1OUT) return;//detect rising edge (C1- > C1+)
		//for (i=0; i<iwait; i++) {}//wait
		while (ADCON0bits.GO_DONE) {}
		iadc = (((int)ADRESH) << 8) | ADRESL;
	    ADCON0bits.CHS0 = 1;// select AN1
	    ADCON0bits.GO_DONE = 1; // start AD ch1
		while (ADCON0bits.GO_DONE) {}
		ibattery = (((int)ADRESH) << 8) | ADRESL;
		iadc -= ibattery;
		ADCON0bits.CHS0 = 0;// back to AN0
		if (bDosemeter) {
			if (iadc > 0x20) return;
			else if (iadc < 0x04) return;
			else jcount++;
		} else {
			isigl += iadc;
			isigh += (long)iadc * (long)iadc;
			jcount++;
		}
		//beep
		if (bBeepOn) {
			T2CONbits.TMR2ON = 1;
			CCP1CONbits.CCP1M3 = 1;//configure CCP1 module in PWM mode
			CCP1CONbits.CCP1M2 = 1;//configure CCP1 module in PWM mode
			//beep timer
			TMR1H = 0x00;
			TMR1L = 0x00;
			T1CONbits.TMR1ON = 1;
		}
	}	//This return will be a "retfie fast", since this is in a #pragma interrupt section 

	#pragma tmpdata	//use the default .tmpdata section
	#pragma interruptlow YourLowPriorityISRCode
	void YourLowPriorityISRCode()
	{
		int isec, imin, i, k;
		if (INTCONbits.TMR0IF) {
			if (bDosemeter) {
				TMR0H = 0x67;
				TMR0L = 0x6a;//preset timer
				isec = icount % 60;
				jcourse[isec] = jcount;
				jtotal += jcount;
				jcount = 0;
				if (isec == 59) {
					imin = iminutes % 10;
					k = 0;
					for (i=0; i<60; i++) {k += jcourse[i];}
					mcourse[imin] = k;
					iminutes++;
				}
			}
			INTCONbits.TMR0IF = 0;
			icount++;
		}
		if (PIR1bits.TMR1IF) {
			PIR1bits.TMR1IF = 0;
			//stop beep timer
			T1CONbits.TMR1ON = 0;
			//stop beep
			T2CONbits.TMR2ON = 0;
			CCP1CONbits.CCP1M3 = 0;//configure CCP1 module in PWM mode
			CCP1CONbits.CCP1M2 = 0;//configure CCP1 module in PWM mode
		}
	}	//This return will be a "retfie", since this is in a #pragma interruptlow section 

#endif //of "#if defined(__18CXX)"

/** D E C L A R A T I O N S **************************************************/
#pragma code
/******************************************************************************
 * Function:        void main(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Side Effects:    None
 * Overview:        Main program entry point.
 * Note:            None
 *****************************************************************************/
void main(void)
{
    InitializeSystem();
    while(1)
    {
        USBTasks();         // USB Tasks
        ProcessIO();        // See user\user.c & .h
    }//end while
}//end main

/******************************************************************************
 * Function:        static void InitializeSystem(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Side Effects:    None
 * Overview:        InitializeSystem is a centralize initialization routine.
 *                  All required USB initialization routines are called from
 *                  here.
 *                  User application initialization routine should also be
 *                  called from here.                  
 * Note:            None
 *****************************************************************************/
static void InitializeSystem(void)
{
    ADCON1 |= 0x0F;                 // Default all pins to digital
    
    //#if defined(USE_USB_BUS_SENSE_IO)
    //tris_usb_bus_sense = INPUT_PIN; // See io_cfg.h
    //#endif
    
    //#if defined(USE_SELF_POWER_SENSE_IO)
    //tris_self_power = INPUT_PIN;
    //#endif
    
    mInitializeUSBDriver();         // See usbdrv.h
    
    UserInit();                     // See user.c & .h

}//end InitializeSystem

/******************************************************************************
 * Function:        void USBTasks(void)
 * PreCondition:    InitializeSystem has been called.
 * Input:           None
 * Output:          None
 * Side Effects:    None
 * Overview:        Service loop for USB tasks.
 * Note:            None
 *****************************************************************************/
void USBTasks(void)
{
    /*
     * Servicing Hardware
     */
    USBCheckBusStatus();                    // Must use polling method
    if(UCFGbits.UTEYE!=1)
        USBDriverService();                 // Interrupt or polling method

}// end USBTasks

//configuration
#if defined(__18F2550)
	#pragma config FOSC = HS 
	//#pragma config FOSC = HSPLL_HS
	#pragma config PLLDIV = 5 
	//#pragma config CPUDIV = OSC4_PLL6 //FOSC=HS: 5 MHz, FOSC=HSPLL: 16 MHz
	#pragma config CPUDIV = OSC2_PLL3 //FOSC=HS: 10 MHz, FOSC=HSPLL: 32 MHz
	//#pragma config CPUDIV = OSC1_PLL2 //FOSC=HS: 20 MHz, FOSC=HSPLL: 48 MHz
	#pragma config USBDIV = 2 
	
	//#pragma config FCMEM = OFF 
	#pragma config FCMEN = OFF
	#pragma config IESO = OFF 
	
	#pragma config PWRT = OFF 
	#pragma config BOR = ON 
	//#pragma config BORV = 28 
	#pragma config BORV = 3
	#pragma config VREGEN = ON 
	
	#pragma config WDT = OFF 
	//#pragma config WDTPS = 1 
	#pragma config WDTPS = 32768
	
	#pragma config MCLRE = OFF 
	//#pragma config MCLRE = ON 
	#pragma config LPT1OSC = OFF 
	#pragma config PBADEN = OFF 
	//#pragma config CCP2MX = OFF
	
	//#pragma config STVREN = OFF 
	#pragma config STVREN = ON
	#pragma config LVP = OFF 
	//#pragma config ICPRT = OFF 
	#pragma config XINST = OFF 
	//#pragma config DEBUG = ON 
	
	#pragma config CP0 = OFF 
	#pragma config CP1 = OFF
	#pragma config CPB = OFF 
	#pragma config WRT0 = OFF 
	#pragma config WRT1 = OFF
	//#pragma config WRTB = ON
	#pragma config WRTB = OFF
	#pragma config WRTC = OFF
	#pragma config EBTR0 = OFF 
	#pragma config EBTR1 = OFF
	#pragma config EBTRB = OFF
	
	#pragma romdata idlocs=0x200000
	unsigned char rom idlocData[8] = "ryuta";
	
	#pragma romdata devid=0x3ffffe
	unsigned char rom devidID[2] = {0x02,0x02};
#elif defined(__18F14K50)
	//#pragma config CPU_DIV = NoClkDiv, USB_LSCLK = OFF				// CONFIG1L
	//#pragma config FOSC = HS, PLL_EN=ON, FCMEN = OFF, IESO = OFF		// CONFIG1H
	//#pragma config PWRT = OFF, BOREN = OFF, BORV = 30, VREGEN = ON	// CONFIG2L
	#pragma config CPUDIV = NOCLKDIV, USBDIV = OFF						// CONFIG1L
	#pragma config FOSC = HS, PLLEN=ON, FCMEN = OFF, IESO = OFF			// CONFIG1H
	#pragma config PWRTEN = OFF, BOREN = OFF, BORV = 30					// CONFIG2L
	#pragma config WDTEN = OFF, WDTPS = 32768							// CONFIG2H
	#pragma config MCLRE = OFF, HFOFST = OFF							// CONFIG3H
	#pragma config STVREN = ON, LVP = OFF, XINST = OFF, BBSIZ=OFF		// CONFIG4L
	#pragma config CP0 = OFF, CP1 = OFF			// CONFIG5L
	#pragma config CPB = OFF					// CONFIG5H
	#pragma config WRT0 = OFF, WRT1 = OFF		// CONFIG6L
	//#pragma config WRTB = ON, WRTC = OFF		// CONFIG6H
	// Disabled WRTB for debugging.  Reenable for real.
	#pragma config WRTB = OFF, WRTC = OFF		// CONFIG6H
	#pragma config EBTR0 = OFF, EBTR1 = OFF		// CONFIG7L
	#pragma config EBTRB = OFF					// CONFIG7H

	//#ifdef __DEBUG
	//	#pragma config BKBUG = ON 
	//#endif
	//#ifndef __DEBUG
	//	#pragma config BKBUG = OFF
	//	#pragma config DEBUG = OFF
	//#endif   
#endif //of "#if defined(__18F2550)"

/** EOF main.c ***************************************************************/
