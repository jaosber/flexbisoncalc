# Definición de variables
CC = gcc
O ?= program

BISON_FILE = grammar.y
LEX_FILE = scanner.l
C_FILES = main.c lex.yy.c grammar.tab.c
PYTHON_SCRIPT = script.py

# Objetivo principal
all: $(O)

# Generación de archivos de Bison
grammar.tab.c grammar.tab.h: $(BISON_FILE)
	bison -d $(BISON_FILE)

# Generación de archivos de Flex (dependiente de Bison)
lex.yy.c lex.yy.h: $(LEX_FILE) grammar.tab.h
	flex --header-file=lex.yy.h $(LEX_FILE)

# Compilación del programa
$(O): $(C_FILES) lex.yy.h grammar.tab.h
	$(CC) -o $(O) $(C_FILES)

# Ejecución del script de Python
runscript:
	python $(PYTHON_SCRIPT)

# Limpieza de archivos generados
clean:
	rm -f lex.yy.c lex.yy.h grammar.tab.c grammar.tab.h $(O) *.output