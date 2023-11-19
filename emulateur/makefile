CFLAGS = -std=c++17
SOURCES = main.cpp
OUTPUT = emulateur

output:
	g++ $(CFLAGS)  $(SOURCES) -o  ../bin/$(OUTPUT)

exec: $(OUTPUT)
	./$(OUTPUT) test
clean: 
	rm $(OUTPUT)