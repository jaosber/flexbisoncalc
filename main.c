#include <stdio.h>
#include <string.h>
#include <math.h>
#include "grammar.tab.h"
#include "lex.yy.h"
#include "utilities.h"

/* globals */
char *yyoriginaltext;

/* functions headers */
char *code_format(const char *text);
void test(void);
void test_input(void);

int main(int argc, char *argv[]){

  //test();
  test_input();

  printf("\nProcess terminated successfully.\n");

  return 0;
}

void test(){

  const char *proto_code = "3.1415*tanh((2.5)\n";

  yyoriginaltext = strdup(proto_code);

  // strncpy(yyoriginaltext, sql_query, yyoriginaltextmaxsize - 1);
  // yyoriginaltext[yyoriginaltextmaxsize - 1] = '\0';

  char *formatted_code = code_format(proto_code);

  if (formatted_code != NULL){
    printf("**** Code *****\n%s\n", formatted_code);
    free(formatted_code); // Liberar la memoria asignada
  }
  else{
    printf("Error formatting code\n");
  }

  YY_BUFFER_STATE bp = yy_scan_string(proto_code);

  // int tok;
  // while(tok = yylex()) { //printf("%d", tok); }
  // yylex();
  yyparse();
  yy_delete_buffer(bp);

  // print_tokens_by_category(tokens, count_token);
  // free_tokens_array(tokens, max_token);
  // count_token = 0;

  free(yyoriginaltext);

  return;
}

void test_input(){

  char input[256];  //Tamaño suficiente para almacenar la entrada del usuario
  
  while (1){
    
    printf("\nEnter an expression (or type 'exit' to quit): ");

    // Leer la entrada del usuario
    if (fgets(input, sizeof(input), stdin) == NULL){
      break; // Salir si hay un error de lectura
    }

    // Eliminar el salto de línea de la entrada
    //input[strcspn(input, "\n")] = '\0';

    // Verificar si el usuario quiere salir
    if (strcmp(input, "exit\n") == 0) { break; }
    //if (strcmp(input, "clear\n") == 0){ continue; }

    // Configurar el buffer de Flex para analizar la entrada del usuario
    yyoriginaltext = strdup(input); // Guardar el texto original en `yyoriginaltext`
    YY_BUFFER_STATE bp = yy_scan_string(input);

    // Ejecutar el análisis sintáctico con Bison
    int parse_result = yyparse();

    if (parse_result == 0){
      //printf("The expression is valid.\n");
    }
    else{
      printf(">> The expression is invalid.\n");
    }

    // Limpiar el buffer de Flex y liberar memoria
    yy_delete_buffer(bp);
    free(yyoriginaltext);

  }

  return;
}

int count_digits(int number){
  return (number > 0) ? (int)log10((double)number) + 1 : 1;
}

char *code_format(const char *text){

  int line = 1;                    // Line counter
  int length = strlen(text);       // Length of the original text
  int estimated_size = length * 2; // Initial estimate for the size of the formatted output

  char *formatted = malloc(estimated_size); // Reserve memory for the output
  
  if (formatted == NULL){
    return NULL; // If memory could not be allocated
  }

  formatted[0] = '\0'; // Initialize the formatted string as empty

  const char *p = text;
  char line_number[20]; // Buffer for the line number

  // Count the number of lines in the text to know the required width for the line numbers
  int total_lines = 1;
  for (const char *q = text; *q; q++){
    if (*q == '\n')
      total_lines++;
  }

  // Calculate the number of digits needed for the largest line number
  int line_width = count_digits(total_lines);

  while (*p)
  {
    // Add line number and separator, using the calculated width
    snprintf(line_number, sizeof(line_number), "%*d | ", line_width, line);
    strcat(formatted, line_number);

    // Add the current line from the original text
    while (*p && *p != '\n')
    {
      strncat(formatted, p, 1);
      p++;
    }

    // Handle the newline character
    if (*p == '\n')
    {
      strcat(formatted, "\n"); // Use strcat instead of strncat
      p++;
    }

    line++; // Increment the line counter

    // Increase the buffer size if necessary
    if (strlen(formatted) + 100 > estimated_size)
    {
      estimated_size *= 2;                            // Double the estimated size
      formatted = realloc(formatted, estimated_size); // Resize the buffer
      if (formatted == NULL)
      {
        return NULL; // If memory reallocation fails
      }
    }
  }

  return formatted;
}

void flush_stdin(){
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
  {
    // no hacer nada, solo consumir los caracteres
  }
}

/*

bison -d grammar.y
flex --header-file=lex.yy.h scanner.l
python script.py
gcc -o program lex.yy.c grammar.tab.c main.c
./program.exe

*/
