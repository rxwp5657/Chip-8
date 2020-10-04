#ifndef DEBUGER_H
#define DEBUGER_H

#include <string>
#include <stdio.h>

#include "./cpu.h"

namespace chip
{
    void print_registers(const CPU& cpu)
    {
        for(int i = 0; i < cpu.V.size(); i++) printf("V[%d]\t%d\n", i, cpu.V[i]);

        printf("----\n");
    }

    void print_sp_registers(const CPU& cpu)
    {
        printf("DT\t%d\n", cpu.DT);
        printf("ST\t%d\n", cpu.ST);
        printf("SP\t%d\n", cpu.SP);
        printf("PC\t%d\n", cpu.PC);
        printf("I \t%d\n", cpu.I);
        printf("K PAD\t%d\n", cpu.key_pad);
        printf("\n");
    }

    void print_stack(const CPU& cpu)
    {
        for(int i = 0; i < cpu.stack.size(); i++) printf("S[%d]\t%d\n", i, cpu.stack[i]);
        printf("\n");
    }

}

#endif