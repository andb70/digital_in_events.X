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
#ifndef _CONFIG_DIGITAL_IO
#define _CONFIG_DIGITAL_IO

#define NUM_DIGITAL_INPUTS      3

#define IN_0_PORT           IOPORT_D
#define IN_0_PIN            2
#define IN_0_ADDRESS        0x01
#define IN_0_EMIT           *ioEdgeSend1

#define IN_1_PORT           IOPORT_D
#define IN_1_PIN            3
#define IN_1_ADDRESS        0x11
#define IN_1_EMIT           *ioEdgeSend2

#define IN_2_PORT           IOPORT_B
#define IN_2_PIN            13
#define IN_2_ADDRESS        0xAB
#define IN_2_EMIT           *ioEdgeSend2

#endif