#pragma once

#include<string>

//instructions
enum Instruction {
    ADD,
    SUB,
    MUL,
    DIV,

    EQL,
    NEQ,
    GTR,
    LSS,
    GEQ,
    LEQ,

    PRN,//print top 
    INN,//read in adrs on top and pull

    INT,

    LDI,//LDI val: push val
    LDA,//LDA adr: push adr

    LDV,//replace top by adr from top
    STO,//store top in the adr under and pull 2x

    BRN,//BRN i JUMP to i
    BZE,// BZE i JUMP to i if top is 0

    HLT//stop
};

enum ProgramBlockType {
    INSTRUCTION,
    CONST
};

struct ProgramBlock {
    int val;
    ProgramBlockType type;
};

//instruction to string (debug)
std::string instructionToString(Instruction inst);