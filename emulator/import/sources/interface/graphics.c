// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      graphics.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      21st November 2023
//      Reviewed :  No
//      Purpose :   Graphics mode manager
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

#include "data/palette.h"  														// Default palette.make

struct GraphicsMode gMode;														// Info about current mode.

static uint8_t graphicsMemory[MAXGRAPHICSMEMORY],								// RAM used for graphics and console text.
				consoleMemory[MAXCONSOLEMEMORY]; 

// ***************************************************************************************
//
//							Initialise the graphics mode 0
//
// ***************************************************************************************

static void GFXInitialise320x240x256(void) {
	gMode.xCSize = 53;gMode.yCSize = 30;										// Console text size
	gMode.xGSize = 320;gMode.yGSize = 240;  									// Graphics pixel size.
	gMode.fontWidth = 6;gMode.fontHeight = 8;  									// Font size on display
	gMode.graphicsMemory = graphicsMemory;  									// Set up memory pointers.
	gMode.consoleMemory = consoleMemory;
	gMode.startMode = (VOIDFUNC)RNDStartMode0;  								// Set up support functions.
	gMode.setPalette = (VOIDFUNC)RNDSetPalette;
}

// ***************************************************************************************
//
//								Change display mode
//
// ***************************************************************************************

void GFXSetMode(int Mode) {
	GFXInitialise320x240x256(); 												// Initialise mode 0
	(*gMode.startMode)(consoleMemory,graphicsMemory);  							// Start it, telling it about memory.
	//
	//		Default palette. For $0x, this is default colour x. For $yx it is default colour y. x doesn't matter
	//		This is for the split layer sprites.
	//
	for (int i = 0;i < 256;i++) {  												// Default (wrong) palette.
		uint8_t c = i;
		int p = ((i < 16) ? i : (i >> 4)) *3; 									// What colour ?
		(*gMode.setPalette)(c,default_palette[p],default_palette[p+1],default_palette[p+2]); 
   }
   CONInitialise(&gMode);  														// Initialise the console.
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
