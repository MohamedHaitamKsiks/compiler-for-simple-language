#include "analyse_syntaxic.h"

void AnalyseSyntaxic::start(std::vector<TokenInfo> _sentence, char* outputFile) {
    currentTokenId = -1;
    sentence = _sentence;
    program();
    /*for (auto id: identifiers) {
        std::cout << id.word << " : " << id.type << std::endl;
    }
    generator.printProgram();*/
    //prepare machine code
    int machineCode[100000];
    int N = 0;
    
    for (auto block: generator.program) {
        machineCode[N] = block.val;
        N++;
    }
    // write output file
    FILE* file = fopen(outputFile, "wb");
    fwrite(&N, sizeof(int), 1, file); 
    fwrite(machineCode, sizeof(int), N, file);
    fclose(file);
    std::cout << std::endl;
}



Token AnalyseSyntaxic::currentToken() {
    return sentence[currentTokenId].token;
}

Token AnalyseSyntaxic::nextToken() {
    currentTokenId++;
    
    TokenInfo current = sentence[currentTokenId];

    //std::cout<< tokenToString(sentence[currentTokenId].token) <<std::endl;
    if (!inDeclaration && current.token == ID_TOKEN) {
        //find identifier
        Identifier id = {current.word};
        int index = findIdentifierIndex(id);

        if (index < 0) {
            throw std::runtime_error(current.word + " not declared!!");
        }
        id.type = identifiers[index].type;
    }
    return current.token;
}

bool AnalyseSyntaxic::testCurrentSymbole(Token token) {
    TokenInfo current = sentence[currentTokenId];
    if (current.token != token) {
        throw std::runtime_error("expected " + tokenToString(token) + " at " + std::to_string(currentTokenId + 1));
    }
    
    return current.token == token;
}

bool AnalyseSyntaxic::testSymbole(Token token) {
    nextToken();
    return testCurrentSymbole(token);
}

/*traitement de regle syntaxique*/
bool AnalyseSyntaxic::program() {
    programBlock();
    testCurrentSymbole(FIN_TOKEN);
    generator.pushInstruction(HLT);
}

//main program block
bool AnalyseSyntaxic::programBlock() {
    while(instruction()) {
        testCurrentSymbole(PV_TOKEN);
    };
    
}

//declare constant
bool AnalyseSyntaxic::consts() {
    // if null
    if (currentToken() != CONST_TOKEN) {
        return false;
    }
    // if const first constant definition
    inDeclaration = true;
    testSymbole(ID_TOKEN);
    addConstId();
    inDeclaration = false;
    testSymbole(AFF_TOKEN);
    testSymbole(NUM_TOKEN);
    
    // LDA adr
    IdentifierIndex address = generator.addIdentifierAdr();

    // LDV val; STO
    int value = std::stoi(sentence[currentTokenId].word);
    generator.pushInstruction(LDI);
    generator.pushConst(value);
    
    testSymbole(PV_TOKEN);
    return true;
}

// variable declaration
bool AnalyseSyntaxic::vars() {
    if (currentToken() != VAR_TOKEN) {
        return false;
    }
    inDeclaration = true;
    testSymbole(ID_TOKEN);
    addVarId();
    //add identifier
    int address = generator.addIdentifierAdr();
    int variableNumber = 1;
    //add more identifiers if nextToken is ,
    while(nextToken() == VIR_TOKEN) {
        testSymbole(ID_TOKEN);
        addVarId();
        generator.addIdentifierAdr();
        variableNumber++;
    }
    inDeclaration = false;
    //declare and affect expression
    if (variableNumber == 1 && currentToken() == AFF_TOKEN) {
        //generate code
        //LDA adr
        generator.pushInstruction(LDA);
        generator.pushConst(address);
        expression();
        generator.pushInstruction(STO);
    }
    testCurrentSymbole(PV_TOKEN);
    return true;
}

//instruction block defined with { .. }
bool AnalyseSyntaxic::instructionsBlock() {
    if (currentToken() != BEGIN_TOKEN) {
        return false;
    }

    //save current offset (variable scope)
    int currentOffset = generator.offset;
    while(instruction()) {
        testCurrentSymbole(PV_TOKEN);
    };
    testCurrentSymbole(END_TOKEN);
    //get back to currentOffset
    int offsetDifference = generator.offset - currentOffset;
    generator.offset = currentOffset;
    //pull stack
    generator.pushInstruction(INT);
    generator.pushConst(-offsetDifference);
    //delete local identifiers
    identifiers.resize(generator.offset + 1);
    nextToken();
    return true;
}

//instruction
bool AnalyseSyntaxic::instruction() {
    nextToken();
    return (instructionsBlock() || assignment() || vars() || consts() ||ifBlock() || whileBlock() || read() || write() ); 
}

//affectation
bool AnalyseSyntaxic::assignment() {
    if (currentToken() != ID_TOKEN) {
        return false;
    }
    modifyId();
    //generate code
    //LDA adr
    Identifier currentIdentifier = {sentence[currentTokenId].word};
    int address = generator.getIdentifierAdr(findIdentifierIndex(currentIdentifier));
    generator.pushInstruction(LDA);
    generator.pushConst(address);

    testSymbole(AFF_TOKEN);
    
    expression();
    generator.pushInstruction(STO);
    return true;
}

//si 
bool AnalyseSyntaxic::ifBlock() {
    if (currentToken() != IF_TOKEN) {
        return false;
    }
    //condition
    condition();
    //jump condition
    generator.pushInstruction(BZE);
    int programAdr = generator.pushConst(-1);
    //instuction
    instruction();
    //set jump location
    ProgramBlock block = {generator.getPC(), CONST};
    generator.setAt(programAdr, block);
    /*else
    if (nextToken() == ELSE_TOKEN) {
        //update new jump to
        block = {generator.getPC() + 2, CONST};
        generator.setAt(programAdr, block);
        // jump outsite of else block when if is true
        generator.pushInstruction(BZE);
        programAdr = generator.pushConst(-1);
        instruction();
        block = {generator.getPC(), CONST};
        generator.setAt(programAdr, block);
    } else {
        
    }*/
    return true;
}

bool AnalyseSyntaxic::whileBlock() {
    if (currentToken() != WHILE_TOKEN) {
        return false;
    }
    int loopStartPC = generator.getPC();
    condition();
    //jump condition
    generator.pushInstruction(BZE);
    int programAdr = generator.pushConst(-1);
    instruction();
    //loop back to condition
    generator.pushInstruction(BRN);
    generator.pushConst(loopStartPC);
    //set jump location
    ProgramBlock block = {generator.getPC(), CONST};
    generator.setAt(programAdr, block);
    return true;
}

//print
bool AnalyseSyntaxic::write() {
    if (currentToken() != WRITE_TOKEN) {
        return false;
    }
    testSymbole(PO_TOKEN);
    expression();
    generator.pushInstruction(PRN);
    while (currentToken() == VIR_TOKEN) {
        expression();
        generator.pushInstruction(PRN);
    }
    testCurrentSymbole(PF_TOKEN);
    nextToken();
    return true;
    
}

//input
bool AnalyseSyntaxic::read() {
    if (currentToken() != READ_TOKEN) {
        return false;
    }
    testSymbole(PO_TOKEN);
    testSymbole(ID_TOKEN);
    modifyId();
    
    //LDA adr
    Identifier currentIdentifier = {sentence[currentTokenId].word};
    int address = generator.getIdentifierAdr(findIdentifierIndex(currentIdentifier));
    generator.pushInstruction(LDA);
    generator.pushConst(address);

    //read
    generator.pushInstruction(INN);

    while (nextToken() == VIR_TOKEN) {
        testSymbole(ID_TOKEN);
        modifyId();

        //LDA adr
        Identifier currentIdentifier = {sentence[currentTokenId].word};
        int address = generator.getIdentifierAdr(findIdentifierIndex(currentIdentifier));
        generator.pushInstruction(LDA);
        generator.pushConst(address);

        //read
        generator.pushInstruction(INN);
    }
    testCurrentSymbole(PF_TOKEN);
    nextToken();
    return true;
}   

//condition
bool AnalyseSyntaxic::condition() {
    testSymbole(PO_TOKEN);
    expression();
    Token op;
    if (!cmpOperator()) {
        throw std::runtime_error("expected valid comparaison operator!");
    }else {
        op = currentToken();
    }
    expression();
    testCurrentSymbole(PF_TOKEN);
    //generer instuction
    switch(op) {
        case EG_TOKEN:
            generator.pushInstruction(EQL);
            break;
        case DIFF_TOKEN:
            generator.pushInstruction(NEQ);
            break;
        case SUP_TOKEN:
            generator.pushInstruction(GTR);
            break;
        case INF_TOKEN:
            generator.pushInstruction(LSS);
            break;
        case SUPEG_TOKEN:
            generator.pushInstruction(GEQ);
            break;
        case INFEG_TOKEN:
            generator.pushInstruction(LEQ);
            break;
    }
}

//est oprateur comparaison
bool AnalyseSyntaxic::cmpOperator() {
    return (currentToken() == EG_TOKEN) || (currentToken() == DIFF_TOKEN) || (currentToken() == INF_TOKEN) || 
    (currentToken() == SUP_TOKEN) || (currentToken() == INFEG_TOKEN) || (currentToken() == SUPEG_TOKEN);
}

//test d'expression
bool AnalyseSyntaxic::expression() {
    nextToken();
    term();
    while(addOperator()) {
        Token op = currentToken();
        nextToken();
        term();
        //ADD/ MIN
        if (op == PLUS_TOKEN)
            generator.pushInstruction(ADD);
        else
            generator.pushInstruction(SUB);

    }
}

bool AnalyseSyntaxic::term() {
    fact();
    nextToken();
    while(multOperator()) {
        Token op = currentToken();
        nextToken();
        fact();
        nextToken();
        //MUL / DIV
        if (op == MULTI_TOKEN)
            generator.pushInstruction(MUL);
        else
            generator.pushInstruction(DIV);
        
    }
}

bool AnalyseSyntaxic::addOperator() {
    return currentToken() == PLUS_TOKEN || currentToken() == MOINS_TOKEN;
}

bool AnalyseSyntaxic::multOperator() {
    return currentToken() == MULTI_TOKEN || currentToken() == DIV_TOKEN;
}

bool AnalyseSyntaxic::fact() {
    if (currentToken() == ID_TOKEN || currentToken() == NUM_TOKEN) {
        if (currentToken() == ID_TOKEN) {
            //LDA adr
            Identifier currentIdentifier = {sentence[currentTokenId].word};
            int address = generator.getIdentifierAdr(findIdentifierIndex(currentIdentifier));
            generator.pushInstruction(LDA);
            generator.pushConst(address);
            generator.pushInstruction(LDV);
        }
        else {
            // LDV val
            int value = std::stoi(sentence[currentTokenId].word);
            generator.pushInstruction(LDI);
            generator.pushConst(value);
        }
        return true;
    }
    testCurrentSymbole(PO_TOKEN);
    expression();
    testCurrentSymbole(PF_TOKEN);
    return true;
}

//analyse sementique
//find identifier index in array
int AnalyseSyntaxic::findIdentifierIndex(Identifier id) {
    for (int i = 0; i < identifiers.size(); i++) {
        if (id.word == identifiers[i].word)
            return i;
    }
    return -1;
}

//add identifier to array
void AnalyseSyntaxic::addIdentifier(Identifier id) {
    int index = findIdentifierIndex(id);
    if (index >= 0) {
        throw std::runtime_error("Identifier " + identifiers[index].word + " is already declared");
        return;
    }
    identifiers.push_back(id);
}


//add const id
void AnalyseSyntaxic::addConstId() {
    Identifier constId{};
    constId.word = sentence[currentTokenId].word;
    constId.type = CONST_ID;
    addIdentifier(constId);
}
//add var id
void AnalyseSyntaxic::addVarId() {
    Identifier varId{};
    varId.word = sentence[currentTokenId].word;
    varId.type = VARS_ID;
    addIdentifier(varId);
    generator.pushInstruction(LDI);
    generator.pushConst(0);
}

void AnalyseSyntaxic::modifyId() {
    //test identifier type
    Identifier id = {sentence[currentTokenId].word};
    id.type = identifiers[findIdentifierIndex(id)].type;
    // check if const
    if (id.type == CONST_ID) {
        throw std::runtime_error("error on " + id.word + "; can't modify consts");
    } 
}