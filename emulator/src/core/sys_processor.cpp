// *******************************************************************************************************************************
// *******************************************************************************************************************************
//
//		Name:		sys_processor.c
//		Purpose:	Processor Emulation.
//		Created:	22nd November 2023
//		Author:		Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************************************************
// *******************************************************************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cstdint>
#include "sys_processor.h"
#include "sys_debug_system.h"
#include "hardware.h"
#include "common.h"

// *******************************************************************************************************************************
//														   Timing
// *******************************************************************************************************************************

#define CYCLE_RATE 		(56*1024*1024/10)											// Cycles per second (5.6Mhz)
#define FRAME_RATE		(60)														// Frames per second (60 arbitrary)
#define CYCLES_PER_FRAME (CYCLE_RATE / FRAME_RATE)									// Cycles per frame

// *******************************************************************************************************************************
//														CPU / Memory
// *******************************************************************************************************************************

static BYTE8 a,x,y,s;																// 6502 A,X,Y and Stack registers
static BYTE8 carryFlag,interruptDisableFlag,breakFlag,								// Values representing status reg
			 decimalFlag,overflowFlag,sValue,zValue;
static WORD16 pc;																	// Program Counter.
static int argumentCount;
static char **argumentList;
static LONG32 cycles;																// Cycle Count.
static int inFastMode = 0; 															// Fast mode flag (speeds up unit testing)

// *******************************************************************************************************************************
//											 Memory and I/O read and write macros.
// *******************************************************************************************************************************

#define Read(a) 	_Read(a)														// Basic Read
#define Write(a,d)	_Write(a,d)														// Basic Write
#define ReadWord(a) (Read(a) | ((Read((a)+1) << 8)))								// Read 16 bit, Basic
#define Cycles(n) 	cycles += (n)													// Bump Cycles
#define Fetch() 	_Read(pc++)														// Fetch byte
#define FetchWord()	{ temp16 = Fetch();temp16 |= (Fetch() << 8); }					// Fetch word

static inline BYTE8 _Read(WORD16 address);											// Need to be forward defined as 
static inline void _Write(WORD16 address,BYTE8 data);								// used in support functions.

#include "6502/__6502support.h"

// *******************************************************************************************************************************
//											   Read and Write Inline Functions
// *******************************************************************************************************************************

BYTE8 *CPUAccessMemory(void) {
	return cpuMemory;
}

static inline BYTE8 _Read(WORD16 address) {
	return cpuMemory[address];
}

static inline void _Write(WORD16 address,BYTE8 data) { 
	 cpuMemory[address] = data;			
	 if (address == CONTROLPORT) {
	 	DSPHandler(cpuMemory+address,cpuMemory);
	 }
}

// *******************************************************************************************************************************
//													Remember Arguments
// *******************************************************************************************************************************

void CPUSaveArguments(int argc,char *argv[]) {
	argumentCount = argc;
	argumentList = argv;
}

// *******************************************************************************************************************************
//														Reset the CPU
// *******************************************************************************************************************************

//#include "binary.h"

int static has_load_address(char* file_name) {
    char* extension = strrchr(file_name, '.');
    if (extension == NULL) {
        return 0;
    }
    return strcmp(extension, ".prg") == 0;
}


void CPUReset(void) {
	char command[128];
	HWReset();																		// Reset Hardware
	for (int i = 1;i < argumentCount;i++) { 										// Look for loads.
		strcpy(command,argumentList[i]);  											// Copy command
		char *pos = strchr(command,'@'); 											// Look for splitting @
		if (pos != NULL) {
			int ch,address,a1;
			*pos++ = '\0'; 															// Split it
			if (sscanf(pos,"%x",&address) != 1)  									// Hex -> Decimal
						exit(fprintf(stderr,"Bad format %s",pos));
			a1 = address;					
			FILE *f = fopen(command,"rb");  										// Read file in and copy to RAM.
			if (f == NULL) exit(fprintf(stderr,"Bad file %s",command));
			while (ch = fgetc(f),ch >= 0) {
				cpuMemory[address] = ch;
				address = (address+1) & 0xFFFF;
			}
			fclose(f);
			printf("Load %s to %x\n",command,a1);
		}
		else {
			if (has_load_address(command)) {
			   
			    FILE *f = fopen(command,"rb"); 
			    uint16_t loadAddress;
				uint32_t currentAddress;

			    if (f == NULL) {
                    exit(fprintf(stderr,"Bad file %s",command));
                }

				if (fread(&loadAddress, sizeof(loadAddress), 1, f) != 1) {
					    printf("Failed to read adress bytes from %s\n", command);
					    fclose(f);
					    exit(1);
				}

                currentAddress = loadAddress;

                while ((fread(&cpuMemory[currentAddress], sizeof(uint8_t), 1, f) == 1) && (currentAddress < 0x10000)) {
                     printf("%04X: %02X\n", currentAddress, cpuMemory[currentAddress]);
					 
					 if(currentAddress == 0xffff)
					     break;
					 currentAddress++;
                }
                fclose(f);

				if (currentAddress < 0x10000) {
				    printf("load: %s [%04X - %04X]\n", command, loadAddress, currentAddress);
				}
				else {
					exit(fprintf(stderr,"Invalid load range:  %s [%04X - %04X]\n",command, loadAddress, currentAddress));
				}

			}
			else {
               exit(fprintf(stderr,"No load address specified:file %s",command));
			}
		}
	}
	resetProcessor();																// Reset CPU	
}

// *******************************************************************************************************************************
//													  Invoke IRQ
// *******************************************************************************************************************************

void CPUInterruptMaskable(void) {
	irqCode();
}

// *******************************************************************************************************************************
//												Execute a single instruction
// *******************************************************************************************************************************

BYTE8 CPUExecuteInstruction(void) {
	if (pc == 0xFFFF) {
		printf("Hit CPU $FFFF - exiting emulator\n");
		CPUExit();
		return FRAME_RATE;
	}
	BYTE8 opcode = Fetch();															// Fetch opcode.
	switch(opcode) {																// Execute it.
		#include "6502/__6502opcodes.h"
	}
	int cycleMax = inFastMode ? CYCLES_PER_FRAME*10:CYCLES_PER_FRAME; 		
	if (cycles < cycleMax) return 0;												// Not completed a frame.
	cycles = 0;																		// Reset cycle counter.
	HWSync();																		// Update any hardware
	return FRAME_RATE;																// Return frame rate.
}

// *******************************************************************************************************************************
//												Read/Write Memory
// *******************************************************************************************************************************

BYTE8 CPUReadMemory(WORD16 address) {
	return Read(address);
}

void CPUWriteMemory(WORD16 address,BYTE8 data) {
	Write(address,data);
}


#include "gfx.h"

// *******************************************************************************************************************************
//		Execute chunk of code, to either of two break points or frame-out, return non-zero frame rate on frame, breakpoint 0
// *******************************************************************************************************************************

BYTE8 CPUExecute(WORD16 breakPoint1,WORD16 breakPoint2) { 
	BYTE8 next;
	do {
		BYTE8 r = CPUExecuteInstruction();											// Execute an instruction
		if (r != 0) return r; 														// Frame out.
		next = CPUReadMemory(pc);
	} while (pc != breakPoint1 && pc != breakPoint2 && next != 0x03);				// Stop on breakpoint or UNOP, which is now break debugger
	return 0; 
}

// *******************************************************************************************************************************
//									Return address of breakpoint for step-over, or 0 if N/A
// *******************************************************************************************************************************

WORD16 CPUGetStepOverBreakpoint(void) {
	BYTE8 opcode = CPUReadMemory(pc);												// Current opcode.
	if (opcode == 0x20) return (pc+3) & 0xFFFF;										// Step over JSR.
	return 0;																		// Do a normal single step
}

void CPUEndRun(void) {
	FILE *f = fopen("memory.dump","wb");
	fwrite(cpuMemory,1,MEMSIZE,f);
	fclose(f);	
}

void CPUExit(void) {	
	printf("Exiting.\n");
	GFXExit();
}


// *******************************************************************************************************************************
//											Retrieve a snapshot of the processor
// *******************************************************************************************************************************

static CPUSTATUS st;																	// Status area

CPUSTATUS *CPUGetStatus(void) {
	st.a = a;st.x = x;st.y = y;st.sp = s;st.pc = pc;
	st.carry = carryFlag;st.interruptDisable = interruptDisableFlag;st.zero = (zValue == 0);
	st.decimal = decimalFlag;st.brk = breakFlag;st.overflow = overflowFlag;
	st.sign = (sValue & 0x80) != 0;st.status = constructFlagRegister();
	st.cycles = cycles;
	return &st;
}

