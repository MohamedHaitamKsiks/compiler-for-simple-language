#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <unistd.h>

#include "lex.h"

class AnalyseLexical {
    private:
        //current word
        std::string currentWord = "";
        Category currentWordType;
        Token currentWordToken;

        //current character
        char currentChar = ' ';

        //file
        FILE* file;

        //get next char
        void nextChar();

        //lexical analysis of current word
        Token lexicalAnalysis();

    //result
    public:
        std::vector<TokenInfo> tokenResults;
        void start(const char* filePath);

};