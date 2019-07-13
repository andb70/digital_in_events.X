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
#include "digital_in.h"
#include "config_digital_in.h"
#include "../timers1.X/timers.h"
#include <plib.h> // Include the PIC32 Peripheral Library

unsigned int oldPort;
#define port_i           (digitalIOMappings[i].port)
#define bit_i            (1 << digitalIOMappings[i].pin)   
#define address_i        (digitalIOMappings[i].address) 

void initDigitalINs(digitalInEvent ioEdgeSend1, digitalInEvent ioEdgeSend2 ){
    #if (NUM_DIGITAL_INPUTS > 0)
        digitalIOMappings[0].port = IN_0_PORT;
        digitalIOMappings[0].pin = IN_0_PIN;
        digitalIOMappings[0].address = IN_0_ADDRESS;
        digitalIOMappings[0].emit = IN_0_EMIT;
    #endif
    #if (NUM_DIGITAL_INPUTS > 1)
        digitalIOMappings[1].port = IN_1_PORT;
        digitalIOMappings[1].pin = IN_1_PIN;
        digitalIOMappings[1].address = IN_1_ADDRESS;
        digitalIOMappings[1].emit = IN_1_EMIT;
    #endif
    #if (NUM_DIGITAL_INPUTS > 2)
        digitalIOMappings[2].port = IN_2_PORT;
        digitalIOMappings[2].pin = IN_2_PIN;
        digitalIOMappings[2].address = IN_2_ADDRESS;
        digitalIOMappings[2].emit = IN_2_EMIT;
    #endif
    #if (NUM_DIGITAL_INPUTS > 3)
        digitalIOMappings[3].port = IN_3_PORT;
        digitalIOMappings[3].pin = IN_3_PIN;
        digitalIOMappings[3].address = IN_3_ADDRESS;
        digitalIOMappings[3].emit = IN_3_EMIT;
    #endif
    #if (NUM_DIGITAL_INPUTS > 4)
        digitalIOMappings[4].port = IN_4_PORT;
        digitalIOMappings[4].pin = IN_4_PIN;
        digitalIOMappings[4].address = IN_4_ADDRESS;
        digitalIOMappings[4].emit = IN_4_EMIT;
    #endif
    #if (NUM_DIGITAL_INPUTS > 5)
        digitalIOMappings[5].port = IN_5_PORT;
        digitalIOMappings[5].pin = IN_5_PIN;
        digitalIOMappings[5].address = IN_5_ADDRESS;
        digitalIOMappings[5].emit = IN_5_EMIT;
    #endif
    #if (NUM_DIGITAL_INPUTS > 6)
        digitalIOMappings[6].port = IN_6_PORT;
        digitalIOMappings[6].pin = IN_6_PIN;
        digitalIOMappings[6].address = IN_6_ADDRESS;
        digitalIOMappings[6].emit = IN_6_EMIT;
    #endif
    #if (NUM_DIGITAL_INPUTS > 7)
        digitalIOMappings[7].port = IN_7_PORT;
        digitalIOMappings[7].pin = IN_7_PIN;
        digitalIOMappings[7].address = IN_7_ADDRESS;
        digitalIOMappings[7].emit = IN_7_EMIT;
    #endif
    #if (NUM_DIGITAL_INPUTS > 8)
        digitalIOMappings[8].port = IN_8_PORT;
        digitalIOMappings[8].pin = IN_8_PIN;
        digitalIOMappings[8].address = IN_8_ADDRESS;
        digitalIOMappings[8].emit = IN_8_EMIT;
    #endif
    #if (NUM_DIGITAL_INPUTS > 9)
        digitalIOMappings[9].port = IN_9_PORT;
        digitalIOMappings[9].pin = IN_9_PIN;
        digitalIOMappings[9].address = IN_9_ADDRESS;
        digitalIOMappings[9].emit = IN_9_EMIT;
    #endif
    #if (NUM_DIGITAL_INPUTS > 10)
        digitalIOMappings[10].port = IN_10_PORT;
        digitalIOMappings[10].pin = IN_10_PIN;
        digitalIOMappings[10].address = IN_10_ADDRESS;
        digitalIOMappings[10].emit = IN_10_EMIT;
    #endif
    #if (NUM_DIGITAL_INPUTS > 11)
        digitalIOMappings[11].port = IN_11_PORT;
        digitalIOMappings[11].pin = IN_11_PIN;
        digitalIOMappings[11].address = IN_11_ADDRESS;
        digitalIOMappings[11].emit = IN_11_EMIT;
    #endif
    #if (NUM_DIGITAL_INPUTS > 12)
        digitalIOMappings[12].port = IN_12_PORT;
        digitalIOMappings[12].pin = IN_12_PIN;
        digitalIOMappings[12].address = IN_12_ADDRESS;
        digitalIOMappings[12].emit = IN_12_EMIT;
    #endif
    #if (NUM_DIGITAL_INPUTS > 13)
        digitalIOMappings[13].port = IN_13_PORT;
        digitalIOMappings[13].pin = IN_13_PIN;
        digitalIOMappings[13].address = IN_13_ADDRESS;
        digitalIOMappings[13].emit = IN_13_EMIT;
    #endif
    #if (NUM_DIGITAL_INPUTS > 14)
        digitalIOMappings[14].port = IN_14_PORT;
        digitalIOMappings[14].pin = IN_14_PIN;
        digitalIOMappings[14].address = IN_14_ADDRESS;
        digitalIOMappings[14].emit = IN_14_EMIT;
    #endif
    #if (NUM_DIGITAL_INPUTS > 15)
        digitalIOMappings[15].port = IN_15_PORT;
        digitalIOMappings[15].pin = IN_15_PIN;
        digitalIOMappings[15].address = IN_15_ADDRESS;
        digitalIOMappings[15].emit = IN_15_EMIT;
    #endif


    // get the port bits and stack them into a unique variable
    #if (NUM_DIGITAL_INPUTS > 0)
        char i;        
        for (i=0;i<NUM_DIGITAL_INPUTS;i++)
        {
            PORTSetPinsDigitalIn(port_i, bit_i);            
            oldPort |= (PORTReadBits(port_i, bit_i) != 0) << i; 
        }       
    #endif
}


void checkDigitalINs(char timerId){
    unsigned int port = 0;
    unsigned int fTrig, rTrig;
    unsigned int i;

    // if we are surveying a debounce after a BTN_[UP or DOWN], simply wait
    // until the period has elapsed and exit here
    if (timer_usRunning(timerId))
        return;
    
    // get the port bits and stack them into a unique variable
    // this way we can hold the value in oldPort and compare the entire
    // range of inputs at once
    for (i=0; i<NUM_DIGITAL_INPUTS; i++)
    {
        port |= (PORTReadBits(port_i, bit_i) != 0) << i; 
    }


    // falling edge: when port is 0 and oldPort is 1
    // it's true when at least 1 input is falling, there could be several inputs
    // in this state and the first that goes off triggers the debouncer, 
    // if any other input goes off during debouncing then it will be triggered
    // when the timer elapses
    fTrig = ~port & oldPort; 
    
    // raising edge: when port is 1 and oldPort is 0
    // works the same of fTrig
    rTrig = port & ~oldPort; 
     
    // important:
    // 1. no matter if it's falling or a raising front: when a input changes 
    //    the debounce takes place
    // 2. if any input goes off and back on or vice-versa during debouncing,
    //    those inputs changes won't be seen

    
    if ((fTrig | rTrig) !=0 & !digitalIOTriggers.debounce){    
        // there is a a change: start deouncing and exit WITHOUT saving
        // the current port to oldPort, this is necessary to check the port
        // status (after the debounce) against the current oldPort:
        // we will trigger an event only if the input will be in the same
        // state we see it now
        digitalIOTriggers.debounce = 1;
        timer_msSet(timerId, 20);
        return;
    }
    
    // the fTrig and rTrig reflect the edges on the inputs and they're valid
    digitalIOTriggers.fTrigAll = fTrig;
    digitalIOTriggers.rTrigAll = rTrig;    
    
    
    // store the current port state to check it next cycle
    oldPort = port;  
    
    // If we are here the timer is OFF or it has just gone off right now
    // set the debounce as done and check if there is any changed input
    if (digitalIOTriggers.debounce)
    {
        digitalIOTriggers.debounce = 0;
        // send events 
        for (i=0; i<NUM_DIGITAL_INPUTS; i++)
        {
            if (IO_fTrig(i))
                digitalIOMappings[i].emit(address_i, 0);
            if (IO_rTrig(i))
                digitalIOMappings[i].emit(address_i, 1);
        }
    }    
    
    
    
    
//    // If we are here the timer is OFF or it just went off right now
//    // set the debounce as done and check if there is any changed input
//    if (digitalIOTriggers.debounce)
//    {
//        digitalIOTriggers.debounce= 0;
//    }
//    else if (fTrig | rTrig ){
//        // there is a a change: start deouncing and exit WITHOUT saving
//        // the current port to oldPort, this is necessary to check the port
//        // status (after the debounce) against the current oldPort:
//        // we will trigger an event only if the input will be in the same
//        // state we see it now
//        digitalIOTriggers.debounce = 1;
//        timer_msSet(timerId,20);
//        return;
//    }
//    // the fTrig and rTrig reflect the edges on the inputs and they're valid
//    digitalIOTriggers.fTrigAll = fTrig;
//    digitalIOTriggers.rTrigAll = rTrig;
//    
//    // store the current port state to check it next cycle
//    oldPort = port;
}


