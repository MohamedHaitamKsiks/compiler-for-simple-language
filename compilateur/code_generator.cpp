#include "code_generator.h"

//push instruction to program
int CodeGenerator::pushInstruction(Instruction inst) {
    ProgramBlock block = {(int) inst, INSTRUCTION};
    program.push_back(block);
    pc++;
    
    return program.size() - 1;
}

//push constant to program
int CodeGenerator::pushConst(int c) {
    ProgramBlock block = {c, CONST};
    program.push_back(block);
    pc++;
    return program.size() - 1;
}

//show code
void CodeGenerator::printProgram() {
    int currentPC = 0;
    for (auto block: program) {
        if (block.type == INSTRUCTION) 
            std::cout << std::endl << currentPC << " " << instructionToString((Instruction) block.val);
        else
            std::cout << " " << block.val;
        currentPC ++;
    }
}

void CodeGenerator::incrementOffset() {
    offset++;
}

int CodeGenerator::addIdentifierAdr() {
    incrementOffset();
    return offset;
}

int CodeGenerator::getIdentifierAdr(IdentifierIndex index) {
    return index;
}

//set at
void CodeGenerator::setAt(int index, ProgramBlock block) {
    program[index] = block;
}
//get pc
int CodeGenerator::getPC() {
    return pc;
}