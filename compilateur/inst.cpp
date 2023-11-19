#include "inst.h"

std::string instructionToString(Instruction inst) {
    std::string instToStr[] = {
        "ADD",
        "SUB",
        "MUL",
        "DIV",
        "EQL",
        "NEQ",
        "GTR",
        "LSS",
        "GEQ",
        "LEQ",
        "PRN",
        "INN",
        "INT",
        "LDI",
        "LDA",
        "LDV",
        "STO",
        "BRN",
        "BZE",
        "HLT"
    };
    return instToStr[inst];
}