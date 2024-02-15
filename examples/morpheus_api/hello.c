/*\
|*| Hello World C example for the Neo6502
|*| SPDX-License-Identifier: CC0-1.0
|*|
|*| This is a very basic C program,
|*| which sounds a beep and displays some text at a precise screen location.
|*| Refer to documents/release/api.pdf for more API functions.
|*|
|*| To compile and run:
|*|   $ export CC65_HOME=/usr/share/cc65
|*|   $ ca65 crt0.s
|*|   $ ar65 a neo6502.lib crt0.o
|*|   $ cl65 --static-locals -C neo6502.cfg -O --cpu 65c02 -m hello.map \
|*|          -o hello.neo hello.c neo6502.lib
|*|   $ ../../bin/neo hello.neo@800 cold
\*/

// CC65 C libs
#include <stdio.h>
#include <stdint.h>

// Neo6502 Morpheus firmware API
#include "morpheus.h"

/*\
|*| Play sound effect - (API Group 8, Function 5)
\*/
static void test_sfx()
{
  NEO.p0 = NEO_SOUND_CH_00;
  NEO.p1 = NEO_SOUND_EFFECT_06;
  NEO_SetFunc(NEO_FN_PLAY_SOUND);
  NEO_Command(NEO_CMD_SOUND);
}

/*\
|*| Set cursor position - (API Group 2, Function 7)
\*/
void SetCursorPosition(uint8_t x_pos, uint8_t y_pos)
{
  NEO.p0 = x_pos;
  NEO.p1 = y_pos;
  NEO_SetFunc(NEO_FN_CURSOR_POS);
  NEO_Command(NEO_CMD_CONSOLE);
}

/*\
|*| Write character to console - (API Group 2, Function 6)
\*/
int write(int /* fildes */, const unsigned char *buf, unsigned count)
{
  while (count--)
  {
    while (NEO_IsBusy())
    {
    }
    NEO.p0 = *buf++;
    NEO_SetFunc(NEO_FN_WRITE_CHAR);
    NEO_Command(NEO_CMD_CONSOLE);
  }

  return 0;
}

void drawLine_1(int x1, int y1, int x2, int y2)
{
  while (NEO_IsBusy())
  {
  } // Waits for the previous command to finish.

  NEO.p0 = x1 & 0xFF; // P0P1
  NEO.p1 = x1 >> 8;
  NEO.p2 = y1 & 0xFF; // P2P3
  NEO.p3 = y1 >> 8;
  NEO.p4 = x2 & 0xFF; // P4P5
  NEO.p5 = x2 >> 8;
  NEO.p6 = y2 & 0xFF; // P6P7
  NEO.p7 = y2 >> 8;

  NEO_SetFunc(2); // Command 2, draw line
  NEO_Command(5); // In group 5 - this write triggers the action.
  while (NEO_IsBusy())
  {
  } // Wait for the command to finish (not strictly required)
}

void drawLine_2(int x1, int y1, int x2, int y2)
{
  while (NEO_IsBusy())
  {
  } // Waits for the previous command to finish.

  NEO.p0p1 = x1;
  NEO.p2p3 = y1;
  NEO.p4p5 = x2;
  NEO.p6p7 = y2;

  NEO_SetFunc(2); // Command 2, draw line
  NEO_Command(5); // In group 5 - this write triggers the action.
  while (NEO_IsBusy())
  {
  } // Wait for the command to finish (not strictly required)
}

void drawLine_3(int x1, int y1, int x2, int y2)
{
  while (NEO_IsBusy())
  {
  } // Waits for the previous command to finish.

  NEO.line.x1 = x1;
  NEO.line.y1 = y1;
  NEO.line.x2 = x2;
  NEO.line.y2 = y2;

  NEO_SetFunc(2); // Command 2, draw line
  NEO_Command(5); // In group 5 - this write triggers the action.
  while (NEO_IsBusy())
  {
  } // Wait for the command to finish (not strictly required)
}

void _neo_draw_line()
{
  while (NEO_IsBusy())
  {
  } // Waits for the previous command to finish.

  NEO_SetFunc(2); // Command 2, draw line
  NEO_Command(5); // In group 5 - this write triggers the action.
  while (NEO_IsBusy())
  {
  } // Wait for the command to finish (not strictly required)
}

#define NEO_DrawLine(px1, py1, px2, py2) \
  do                                  \
  {                                   \
    NEO.line.x1 = px1;                \
    NEO.line.y1 = py1;                \
    NEO.line.x2 = px2;                \
    NEO.line.y2 = py2;                \
    _neo_draw_line();                    \
  } while (0);

int main()
{
  // test_sfx();

  SetCursorPosition(0, 22);
  puts("                   Hello world!");
  puts("                                                    "); // 52 blanks

  NEO_DrawLine(10, 10, 100, 100);

  return 0;
}
