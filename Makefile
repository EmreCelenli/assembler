SOURCES=main.c assembler.c parser.c symbolTable.c
EXECUTABLE=assembler

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	gcc -o $(EXECUTABLE) $(SOURCES)

clean:
	rm -f $(EXECUTABLE) *.hack
