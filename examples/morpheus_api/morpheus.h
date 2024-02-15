/*\
|*| Convenience macros for Neo6502 applications programming
|*| SPDX-License-Identifier: CC0-1.0
\*/

struct __morpheus
{
    uint8_t cmd_grp;
    uint8_t fn;
    uint8_t error;
    uint8_t status;
    union
    {
        uint8_t par8[8];
        uint16_t par16[4];
        struct
        {
            uint8_t p0;
            uint8_t p1;
            uint8_t p2;
            uint8_t p3;
            uint8_t p4;
            uint8_t p5;
            uint8_t p6;
            uint8_t p7;
        };
        struct
        {
            uint16_t p0p1;
            uint16_t p2p3;
            uint16_t p4p5;
            uint16_t p6p7;
        };
        struct 
        {
            uint16_t x1;
            uint16_t y1;
            uint16_t x2;
            uint16_t y2;
        }line;
    };
};

#define NEO (*(struct __morpheus *)0xFF00)

#define NEO_Command(command)   \
    {                          \
        NEO.cmd_grp = command; \
    }
#define NEO_SetFunc(function) \
    {                         \
        NEO.fn = function;    \
    }
#define NEO_IsBusy() (NEO.cmd_grp)
#define NEO_GetError() (NEO.error)
#define NEO_GetStatus() (NEO.status)

/* Neo6502 Kernel API control codes (see documents/release/api.pdf) */

// Console functions (Group 2)
uint8_t NEO_CMD_CONSOLE = (uint8_t)0x02;   // API function group ID
uint8_t NEO_FN_WRITE_CHAR = (uint8_t)0x06; // API function ID
uint8_t NEO_FN_CURSOR_POS = (uint8_t)0x07; // API function ID
// Sound functions (Group 8)
uint8_t NEO_CMD_SOUND = (uint8_t)0x08;       // API function group ID
uint8_t NEO_FN_PLAY_SOUND = (uint8_t)0x05;   // API function ID
uint8_t NEO_SOUND_CH_00 = (uint8_t)0x00;     // API function parameter ID
uint8_t NEO_SOUND_EFFECT_06 = (uint8_t)0x06; // API function parameter ID
