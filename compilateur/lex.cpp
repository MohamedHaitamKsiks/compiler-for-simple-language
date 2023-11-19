#include "lex.h"

bool isLetter(char c) {
    return (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'));
}

bool isNumber(char c) {
    return ( '0' <= c && c <= '9');
}

bool isSpecial(char c) {
    //special character
    std::vector<char> SPECIAL_CHARACTERS = {
        ';', 
        '.',
        '+',
        '-',
        '*',
        '/',
        ',',
        ':',
        '=',
        '<',
        '>',
        '(',
        ')',
        '{',
        '}',
        '!'
    };
    return std::count(SPECIAL_CHARACTERS.begin(), SPECIAL_CHARACTERS.end(), c);
}

bool isSeparator(char c) {
    //separators
    std::vector<char> SEPARATORS = {
        ' ', 
        '\t',
        '\n',
        '\r'
    };
    return std::count(SEPARATORS.begin(), SEPARATORS.end(), c);
}


std::string tokenToString(Token token) {
    // string value of token (print purpose)
    std::string TOKEN_TO_STRING[] = {
        //les mots cles
        "CONST_TOKEN",
        "VAR_TOKEN",
        "BEGIN_TOKEN",
        "END_TOKEN",
        "IF_TOKEN",
        "ELSE_TOKEN",
        "WHILE_TOKEN",
        "FOR_TOKEN",
        "READ_TOKEN",
        "WRITE_TOKEN",
        // les symboles speciaux
        "PV_TOKEN",
        "PT_TOKEN",
        "PLUS_TOKEN",
        "MOINS_TOKEN",
        "MULTI_TOKEN",
        "DIV_TOKEN",
        "VIR_TOKEN",
        "AFF_TOKEN",
        "EG_TOKEN",
        "INF_TOKEN",
        "INFEG_TOKEN",
        "SUP_TOKEN",
        "SUPEG_TOKEN",
        "DIFF_TOKEN",
        "PO_TOKEN",
        "PF_TOKEN",
        "FIN_TOKEN",
        //REGLES LEXICAL
        "ID_TOKEN",
        "NUM_TOKEN",
        //symboles errones
        "ERREUR_TOKEN"
  
    };
    return TOKEN_TO_STRING[token];
}

Token getKeywordToken(std::string word) {
    std::unordered_map<std::string, Token> keywords = {
        {"const", CONST_TOKEN},
        {"var", VAR_TOKEN},
        {"if", IF_TOKEN},
        {"else", ELSE_TOKEN},
        {"simo", WHILE_TOKEN},
        {"for", FOR_TOKEN},
        {"input", READ_TOKEN},
        {"print", WRITE_TOKEN}
    };
    //if doesn't exist
    if (keywords.find(word) == keywords.end()) {
        return ERREUR_TOKEN;
    }
    return keywords[word];
}

Token getSymbolToken(std::string word) {
    std::unordered_map<std::string, Token> symbols = {
        {";", PV_TOKEN},
        {".", PT_TOKEN},
        {"+", PLUS_TOKEN},
        {"-", MOINS_TOKEN},
        {"*", MULTI_TOKEN},
        {"/", DIV_TOKEN},
        {",", VIR_TOKEN},
        {"=", AFF_TOKEN},
        {"==", EG_TOKEN},
        {"<", INF_TOKEN},
        {"<=", INFEG_TOKEN},
        {">", SUP_TOKEN},
        {">=", SUPEG_TOKEN},
        {"!=", DIFF_TOKEN},
        {"(", PO_TOKEN},
        {")", PF_TOKEN},
        {"{", BEGIN_TOKEN},
        {"}", END_TOKEN}
    };
    //if doesn't exist
    if (symbols.find(word) == symbols.end()) {
        return ERREUR_TOKEN;
    }
    return symbols[word];
}