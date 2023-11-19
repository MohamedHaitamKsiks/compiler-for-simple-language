#include <vector>
#include <stdexcept>
#include <iostream>
#include <string>
#include "lex.h"
#include "code_generator.h"
#include <cstdlib>

class AnalyseSyntaxic {
    
    std::vector<TokenInfo> sentence;
    int currentTokenId;

    //analyse sementique
    std::vector<Identifier> identifiers;
    bool inMainInstructionBlock = false;
    int pile = 0;

    public:
        void start(std::vector<TokenInfo> _sentence, char* outputFile);

    private:
        //generation du code
        CodeGenerator generator{};
        //in declaration
        bool inDeclaration = false;
        //go to next token and return value
        Token nextToken();
        //get current token
        Token currentToken();
        //go to next token and test it's value
        bool testSymbole(Token token);
        // test current token value
        bool testCurrentSymbole(Token token);

        /*traitement de regle syntaxique*/
        //main program
        bool program();
        //main program block
        bool programBlock();
        
        bool consts();
        bool vars();
        bool instructionsBlock();
        void affec();

        bool instruction();
        bool assignment();
        bool ifBlock();
        bool whileBlock();
        bool read();
        bool write();
        bool condition();
        bool cmpOperator();
        bool expression();
        bool term();
        bool addOperator();
        bool multOperator();
        bool fact();

        //analyse sementique
        //find identifier index in array
        int findIdentifierIndex(Identifier id);
        //add identifier to array
        void addIdentifier(Identifier id);
        //add program id
        void addProgramId();
        //add const id
        void addConstId();
        //add var id
        void addVarId();
        //nodify identifier
        void modifyId();


};