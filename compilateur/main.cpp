#include <iostream>
#include "analyse_lexical.h"
#include "analyse_syntaxic.h"


int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "error: no file given" << std::endl;
        return 1;
    }

    AnalyseLexical analiseurLexical{};
    analiseurLexical.start(argv[1]);

    AnalyseSyntaxic analiseurSyntaxique{};
    analiseurSyntaxique.start(analiseurLexical.tokenResults, argv[2]);

    
    
    return 0;

}