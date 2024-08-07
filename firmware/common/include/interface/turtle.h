// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      turtle.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      26th January 2024
//      Reviewed :  No
//      Purpose :   Turtle graphics header
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _TURTLE_H
#define _TURTLE_H

void TTLInitialise(uint8_t spriteID);
void TTLRotate(int16_t turn);
void TTLForward(int16_t distance,uint8_t colour,uint8_t drawLine);
void TTLSetVisible(bool isVisible);
void TTLHome(void);
void TTLUpdate(void);

#endif

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
