#include "analyse_lexical.h"

//get next char
void AnalyseLexical::nextChar() {
    currentChar = fgetc(file);
    
}

//lexical analysis of current word
Token AnalyseLexical::lexicalAnalysis() {
    //start of current word
    if (currentWord.length() == 0) {
        if (isLetter(currentChar)) {
            currentWordType = Category::WORD;
        } else if (isNumber(currentChar)) {
            currentWordType = Category::NUMER;
        } else if (isSpecial(currentChar)) {
            currentWordType = Category::SPECIAL;
        } else {
            return ERREUR_TOKEN;
        }
    } 
    
    else {
        //if is word
        if (currentWordType == Category::WORD) {
            if (isSpecial(currentChar) || isSeparator(currentChar)) {
                Token token = getKeywordToken(currentWord);
                if (token == ERREUR_TOKEN)
                    token = ID_TOKEN;
                return token;
            }
        }
        // is number
        else if (currentWordType == Category::NUMER) {
            if (!isNumber(currentChar)) {
                return NUM_TOKEN;
            }
        }
        // is special character
        else {
            if (!isSpecial(currentChar) || currentChar == ';' || currentChar == ','  || currentChar == '(' || currentChar == ')' || currentChar == '{' || currentChar == '}') {
                Token token = getSymbolToken(currentWord);
                    if (token != ERREUR_TOKEN) {
                        return token;
                    }
                    return ERREUR_TOKEN;
                
                }

        }
        

    }
    currentWord += currentChar;
    nextChar();
    return lexicalAnalysis();
}

void AnalyseLexical::start(const char* filePath) {
    //open file
    file = fopen(filePath,"r");
    if (!file) {
        std::cout << "error: cannot open given file" << std::endl;
        return;
    }

    //read file
    Token currentToken = ERREUR_TOKEN;
    nextChar();
    //start reading file
    while (currentToken != FIN_TOKEN) {
        while(isSeparator(currentChar)) {
            nextChar();
        }
        if (currentChar == EOF) {
            currentToken = FIN_TOKEN;
        } else {
            currentToken = lexicalAnalysis();
        }
        //create info
        TokenInfo info{};
        info.token = currentToken;
        info.word = currentWord;
        //push info
        tokenResults.push_back(info);
        //std::cout << tokenResults.size() << ": " << currentWord << " => " << tokenToString(currentToken) << std::endl;
        currentWord = "";
        //sleep(2);

    }

    //close file
    fclose(file);
}

