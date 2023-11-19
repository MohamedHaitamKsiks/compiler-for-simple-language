CFLAGS = -std=c++17
SOURCES = analyse_lexical.cpp lex.cpp main.cpp analyse_syntaxic.cpp inst.cpp code_generator.cpp
OUTPUT = compiler

output:
	g++ $(CFLAGS)  $(SOURCES) -o  ../bin/$(OUTPUT)

exec: $(OUTPUT)
	./$(OUTPUT) tests/test.txt test
clean: 
	rm $(OUTPUT)