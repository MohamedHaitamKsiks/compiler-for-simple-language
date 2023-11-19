#include <iostream>
#include <vector>
#include <cstdlib>

#include "inst.h"

#define N 30000

//machine virtuel
int memory[N];
int program[N];
int size;


int pc = 0;
int st = -1;


int main(int argc, char* argv[]) {
    //check for file in params
    if (argc < 2) {
        std::cout << "error: no file given" << std::endl;
        return 1;
    }

    //read program from file
    FILE* file = fopen(argv[1], "rb");
    fread(&size, sizeof(int), 1, file);
    size++;
    fread(&program, sizeof(int) * (size), 1, file);

    bool res;
    int adr;
    int readData;

    //go thro program
    while (program[pc] != HLT) {
        /*for (int i = 0; i <= st; i++) {
            std::cout << memory[i] << ",";
        }
        std::cout << "\n";
        std::cout << "PC " << pc << '\n';
        std::cout << "INST:  " << instToStr[program[pc]] << '\n';
        std::cout << "ST " << st << "\n\n";*/

        

        switch(program[pc]) {
            case ADD:
                memory[st - 1] += memory[st];
                st--;
                break;
            case SUB:
                memory[st - 1] -= memory[st];
                st--;
                break;
            case MUL:
                memory[st - 1] *= memory[st];
                st--;
                break;
            case DIV:
                memory[st - 1] /= memory[st];
                st--;
                break;

            case EQL:
                res = memory[st - 1] == memory[st];
                st--;
                memory[st] = (int)res;
                break;
            case NEQ:
                res = memory[st - 1] != memory[st];
                st--;
                memory[st] = (int)res;
                break;
            case GTR:
                res = memory[st - 1] > memory[st];
                st--;
                memory[st] = (int)res;
                break;
            case LSS:
                res = memory[st - 1] < memory[st];
                st--;
                memory[st] = (int)res;
                break;  
            case GEQ:
                res = memory[st - 1] >= memory[st];
                st--;
                memory[st] = (int)res;
                break;
            case LEQ:
                res = memory[st - 1] <= memory[st];
                st--;
                memory[st] = (int)res;
                break;

            case PRN:
                std::cout << memory[st] << '\n';
                st--;
                break;
            case INN:
                std::cout << "> ";
                
                std::cin >> readData;
                memory[memory[st]] = readData;
                st--;
                break;

            case INT:
                pc++;
                st += program[pc];
                break;

            case LDI:
                st++;
                pc++;
                memory[st] = program[pc];
                break;
            case LDA:
                st++;
                pc++;
                memory[st] = program[pc];
                break;

            case LDV:
                adr = memory[st];
                memory[st] = memory[adr];
                break;
            case STO:
                memory[memory[st-1]] = memory[st];
                st -= 2;
                break;

            case BRN:
                pc++;
                pc = program[pc] - 1;
                break;
            case BZE:
                if (memory[st] == 0) {
                    pc ++;
                    pc = program[pc] - 1;
                    st --;
                }
                break;

        }
        pc++;
    }

}