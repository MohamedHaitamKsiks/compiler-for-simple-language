#pragma once

#include <vector>
#include <iostream>
#include "inst.h"
#include "inst.h"
#include "lex.h"

class CodeGenerator {
    public:
        std::vector<ProgramBlock> program;
        int offset = -1;
        //push instruction to program
        int pushInstruction(Instruction inst);
        //push constant to program
        int pushConst(int c);
        //show code
        void printProgram();
        //inc offset
        void incrementOffset();
        //identifier
        int addIdentifierAdr();
        int getIdentifierAdr(IdentifierIndex index);
        //set at
        void setAt(int index, ProgramBlock block);
        //get pc
        int getPC();

    private:
        int pc = 0;
        //program
        

};