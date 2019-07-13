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

/*  PORT MAPPING  */
/*	CONnector, PIN, PORT, A-map, B-map      mapping table
 * 					
 *	Pin 	Signal Name		PORT        A           B
 *   ------+---------------+-----------+-----------+-----------------------------
 *  CON3
 *  1       RE0/PMD0        RE0
 *  ..
 *  8       RE7/PMD7        RE7
 *  9                       RD1         LED2 (YELLOW)
 * 
 *  UEXT
 *  1       3.3V
 *  2       GND
 *  3       TX2             RF5         UART2 TX
 *  4       RX2             RF4         UART2 RX
 *  5       A5(SCL1)        RD10
 *  6       A4(SDA1)        RD9
 *  7       D12(MISO)       RG7
 *  8       D11(MOSI)       RG8
 *  9       D13(SCK/LED1)   RG6         LED1 (GREEN)
 * 10       UEXT_#CS
 * 
 *	CON4    DIGITAL:	
 *	1       D0(RXD1)        RD2         UART1 RX
 *	2       D1(TXD1)        RD3         UART1 TX
 *	3       D2(BUT)         RD4(+RD0)   BUT
 *	4       D3              RD5         IO_0
 *	5       D4              RD6         IO_1
 *	6       D5              RD7         IO_2
 *	7       D6              RD8         IO_3
 *	8       D7              RD11        IO_4
 *		  |               |           |           |			
 *	CON5 	
 *	1       D8_MMC_#SS      RB13        IO_5
 *	2       D9              RB14        IO_6
 *	3       D10(#SS)        RG9         IO_7
 *	4       D11(MOSI)       RG8         IO_8
 *	5       D12(MISO)       RG7         IO_9
 *	6       D13(SCK/LED1)	RG6			LED1 (GREEN)
 *	7       GND				
 *	8       AREF	
 *		  |               |           |           |				
 *	CON2   ANALOG:			
 *	1       A0              RB1			IO_10
 *	2       A1              RB2			IO_11
 *	3       A2              RB3			IO_12
 *	4       A3              RB4			IO_13
 *	5       A4(SDA1)		RD9         IO_14
 *	6       A5(SCL1)		RD10		IO_15
 * 
 ************************************************** 
 ************************************************** 
 ***                                            ***     
 ***    2       4       6       8       10      ***
 ***    GND     RXD     SDA     MOSI    SSEL    ***
 ***                                            ***
 ***    VCC     TXD     SCL     MISO    SCK     ***
 ***    1       3       5       7       9       ***
 ***                                            ***   
 **** RED ********             ******************** 
 *****************             ******************** 
   
 *****************             ******************** 
 *****************             ******** RED ******* 
 ***                                            ***     
 ***    9       7       5       3       1       ***
 ***    SCK     MISO    SCL     TXD     VCC     ***
 ***                                            ***
 ***    SSEL    MOSI    SDA     RXD     GND     ***
 ***    10      8       6       4       2       ***
 ***                                            ***  
 ************************************************** 
 ************************************************** 
 * 
 */

#ifndef _MODULE_IO
#define _MODULE_IO

#include <plib.h> // Include the PIC32 Peripheral Library

union _digitaIN {
    struct {
        unsigned fTrig_0: 1;     // falling edge
        unsigned fTrig_1: 1;     
        unsigned fTrig_2: 1;     
        unsigned fTrig_3: 1;     
        unsigned fTrig_4: 1;     
        unsigned fTrig_5: 1;     
        unsigned fTrig_6: 1;     
        unsigned fTrig_7: 1;     
        unsigned fTrig_8: 1;     
        unsigned fTrig_9: 1;     
        unsigned fTrig_10: 1;     
        unsigned fTrig_11: 1;     
        unsigned fTrig_12: 1;     
        unsigned fTrig_13: 1;     
        unsigned fTrig_14: 1;     
        unsigned fTrig_15: 1;    
        
        unsigned rTrig_0: 1;     // raising edge
        unsigned rTrig_1: 1;     
        unsigned rTrig_2: 1;     
        unsigned rTrig_3: 1;     
        unsigned rTrig_4: 1;     
        unsigned rTrig_5: 1;     
        unsigned rTrig_6: 1;     
        unsigned rTrig_7: 1;     
        unsigned rTrig_8: 1;     
        unsigned rTrig_9: 1;     
        unsigned rTrig_10: 1;     
        unsigned rTrig_11: 1;     
        unsigned rTrig_12: 1;     
        unsigned rTrig_13: 1;     
        unsigned rTrig_14: 1;     
        unsigned rTrig_15: 1;    
    };
    struct {
        unsigned fTrigAll: 16; // all inputs falling edge
        unsigned rTrigAll: 16; // all inputs raising edge        
        unsigned :7;// pad
        unsigned debounce: 1; // true for few ms after a BTN_DOWN or BTN_UP 
    };
} digitalIOTriggers;


#define IO_port(id)     (digitalIOMappings[id].port)
#define IO_pin(id)      (digitalIOMappings[id].pin)
#define IO_fTrigAll()   (digitalIOTriggers.fTrigAll!=0)
#define IO_rTrigAll()   (digitalIOTriggers.rTrigAll!=0)

#define IO_fTrig(id)    (digitalIOTriggers.fTrigAll >> id & 1)
#define IO_rTrig(id)    (digitalIOTriggers.rTrigAll >> id & 1)

//https://cs.nyu.edu/courses/spring12/CSCI-GA.3033-014/Assignment1/function_pointers.html
// declare a type (pointer to function_name)(with arguments)
// then declare a variable of type (pointer to function_name)
// 
// store into the variable the pointer to a function of type:
// (pointer to function_name) myVar
//
// call the function using the variable
// myVar(with arguments)
// see example of data structure holding the (pointer to function_name) 
// _IOPortMapping.emit
//      digitalIOMappings[i].emit(address_i, 0);
// where address_i is unsigned int, as defined in the typedef:
typedef void (* digitalInEvent)(unsigned int, unsigned int);

// define the callback functions
void ioEdgeSend1(unsigned int address, unsigned int value);
void ioEdgeSend2(unsigned int address, unsigned int value);


struct _IOPortMapping{
    IoPortId port;
    unsigned int pin;
    unsigned int address;
    digitalInEvent emit;
    
} digitalIOMappings[16];

void initDigitalINs(digitalInEvent ioEdgeSend1, digitalInEvent ioEdgeSend2 );

// BUTTONS
void checkDigitalINs(char timerId);

#endif