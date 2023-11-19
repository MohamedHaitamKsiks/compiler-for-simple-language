#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

//tokens
enum Token {
    //les mots cles
    CONST_TOKEN,
    VAR_TOKEN,
    BEGIN_TOKEN,
    END_TOKEN,
    IF_TOKEN,
    ELSE_TOKEN,
    WHILE_TOKEN,
    FOR_TOKEN,
    READ_TOKEN,
    WRITE_TOKEN,
    // les symboles speciaux
    PV_TOKEN,
    PT_TOKEN,
    PLUS_TOKEN,
    MOINS_TOKEN,
    MULTI_TOKEN,
    DIV_TOKEN,
    VIR_TOKEN,
    AFF_TOKEN,
    EG_TOKEN,
    INF_TOKEN,
    INFEG_TOKEN,
    SUP_TOKEN,
    SUPEG_TOKEN,
    DIFF_TOKEN,
    PO_TOKEN,
    PF_TOKEN,
    FIN_TOKEN,
    //REGLES LEXICAL
    ID_TOKEN,
    NUM_TOKEN,
    //symboles errones
    ERREUR_TOKEN
};


struct TokenInfo {
    Token token;
    std::string word;
};

enum IndentifierType {
    CONST_ID,
    VARS_ID
};

struct Identifier {
    std::string word;
    IndentifierType type;
};

typedef int IdentifierIndex;


//category of word
enum Category {
    WORD,
    NUMER,
    SPECIAL
} ;


bool isLetter(char c);

bool isNumber(char c);

bool isSpecial(char c);

bool isSeparator(char c);

std::string tokenToString(Token token);

Token getKeywordToken(std::string word);

Token getSymbolToken(std::string word);