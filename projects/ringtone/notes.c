#include <stdint.h>
#include <stdio.h>

struct note
{
    char name;
    uint8_t index;
};


struct note notes[]=

{
    {'a', 9},
    {'b', 11},
    {'c', 0},
    {'d', 2},
    {'e', 4},
    {'f', 5},
    {'g', 7},

};


void main()
{

    for(int i=0;i<7;i++)
    {
        printf("%c,%d,%d --> %d\n", notes[i].name, notes[i].name, notes[i].index, notes[i].name-96);
    }

    for(int i=0;i<7;i++)
    {
        printf("notes(%d)=%d\n", notes[i].name-96, notes[i].index);
    }

}