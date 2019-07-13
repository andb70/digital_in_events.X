/*
 * released under GNU AFFERO GENERAL PUBLIC LICENSE Version 3, 19 November 2007
 * 
 *      Author:     Andrea Biasutti
 *      Date:       July 12th 2019
 *      Hardware:   PIC32MX440256H
 * 
 * source available here https://github.com/andb70/digitalIN_events.X
 * 
 */
#pragma config FPLLIDIV = DIV_2 // PLL Input Divider (1x Divider)
#pragma config FPLLMUL = MUL_20 // PLL Multiplier (24x Multiplier)
#pragma config UPLLIDIV = DIV_2 // USB PLL Input Divider (12x Divider)
#pragma config UPLLEN = OFF // USB PLL Enable (Disabled and Bypassed)
#pragma config FPLLODIV = DIV_1 // System PLL Output Clock Divider (PLL Divide by 256)
// DEVCFG1
#pragma config FNOSC = PRIPLL // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
#pragma config FSOSCEN = ON // Secondary Oscillator Enable (Enabled)
#pragma config IESO = ON // Internal/External Switch Over (Enabled)
#pragma config POSCMOD = HS // Primary Oscillator Configuration (HS osc mode)
#pragma config OSCIOFNC = ON // CLKO Output Signal Active on the OSCO Pin (Enabled)
#pragma config FPBDIV = DIV_8 // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/8)
#pragma config FCKSM = CSDCMD // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576 // Watchdog Timer Postscaler (1:1048576)
#pragma config FWDTEN = OFF // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
// DEVCFG0
#pragma config DEBUG = OFF // Background Debugger Enable (Debugger is disabled)
#pragma config ICESEL = ICS_PGx2 // ICE/ICD Comm Channel Select (ICE EMUC2/EMUD2 pins shared with PGC2/PGD2)
#pragma config PWP = OFF // Program Flash Write Protect (Disable)
#pragma config BWP = OFF // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF // Code Protect (Protection Disabled)

#define SYS_FREQ 			(80000000L)


// nella slide le righe vanno a capo, nel codice ogni #define va su un?unica riga
#include <p32xxxx.h> // Include PIC32 specifics header file
#include <plib.h> // Include the PIC32 Peripheral Library
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "../timers1.X/timers.h"

// configuration file for digital_in module, contains useful definitions
// configuration data is hard-wired
#include "config_digital_in.h"

// digital inputs managements module: use the initDigitalINs to pass pointers
// to callback functions used to notify events at every input change
// anti-bounce is implemented in the module
#include "digital_in.h"

// LEDS
// on board GREEN
#define monitorGLed(state)  LATGbits.LATG6 = state
#define monitorGLedToggle() LATGbits.LATG6 = !LATGbits.LATG6

// on board YELLOW
#define monitorYLed(state)  LATDbits.LATD1 = state
#define monitorYLedToggle() LATDbits.LATD1 = !LATDbits.LATD1

// Red led CONN 4 - D6
#define monitorRLed(state)  LATDbits.LATD8 = state
#define monitorRLedToggle() LATDbits.LATD8 = !LATDbits.LATD8
void initLeds();

// GLOBALS
void initGlobals();

int main() {
    
    initLeds();
    initTimers();
    // Initialize port pins to be used with display

    initGlobals();
    initDigitalINs(* ioEdgeSend1, * ioEdgeSend2 );
    
    PORTSetPinsDigitalIn(IOPORT_B, BIT_13); // btn
    unsigned int i=0;
    
    while (1) 
    {
        checkTimers();
        checkDigitalINs(0);

        // if we don't use events then this is the code
//        if (PORTReadBits(IOPORT_B, BIT_13) == 0)
//        {
//            i=0;
//        }
//        if (IO_fTrig(0))
//        {
//            i=0; 
//            monitorGLedToggle();
//        }
//        if (IO_fTrig(1))
//        {
//            i=1; 
//            monitorYLedToggle();
//        }
//        if (IO_fTrig(2))
//        {
//            i=1; 
//            monitorRLedToggle();
//        }
//        

    }
    return 1;
}

void initLeds() {
    mJTAGPortEnable(0); // Disable JTAG --> let the red led work
    PORTSetPinsDigitalOut(IOPORT_D, BIT_8); //  RED led @ CON4-D6
    PORTSetPinsDigitalOut(IOPORT_D, BIT_1); //  YELLOW led on-board
    PORTSetPinsDigitalOut(IOPORT_G, BIT_6); //  GREEN  led on-board    
}

void initGlobals() {
    // default state: all off
    LATB = 0;
    LATD = 0;
    LATG = 0;
}

// if we DO use events then this is the code: inputs are configured to have
// different behaviors and consequently fire different events or, that's the same,
// call different callback functions
void ioEdgeSend1(unsigned int address, unsigned int value){
    // toggle when press and toggle when release
    monitorRLed(value);    
}

void ioEdgeSend2(unsigned int address, unsigned int value){
    // toggle only when press, do nothing when release
    if (address == IN_1_ADDRESS & value == 0)
    {
        monitorYLedToggle();
    }
    if (address == IN_2_ADDRESS & value == 0)
    {
        monitorGLedToggle();
    }
}
