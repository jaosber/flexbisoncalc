#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_STACK 1000

typedef struct {
  int line;
  int column;
} Position;

typedef struct {
  Position positions[MAX_STACK];
  int top;
} Stack;

static inline char *to_uppercase(const char *str) {
  
  char *upper_str = strdup(str);

  if (upper_str == NULL) {
    return NULL; 
  }

  for (int i = 0; upper_str[i] != '\0'; i++) {
    upper_str[i] = toupper(upper_str[i]);
  }

  return upper_str;
}

static inline char *to_lowercase(const char *str) {
  
  //duplicate the input string to avoid modifying the original
  char *lower_str = strdup(str);

  if (lower_str == NULL) {
    return NULL; 
  }

  //iterate over each character and convert to lowercase
  for (int i = 0; lower_str[i] != '\0'; i++) {
    lower_str[i] = tolower(lower_str[i]);
  }

  return lower_str;
}

static inline char *replace_all(const char *str, const char *searchvalue, const char *newvalue) {
    // Calculate the lengths of the strings
    int str_len = strlen(str);
    int searchvalue_len = strlen(searchvalue);
    int newvalue_len = strlen(newvalue);

    // Handle the case when searchvalue is an empty string
    if (searchvalue_len == 0) {
        // Return a copy of the original string
        char *result = malloc(str_len + 1);
        if (result == NULL) {
            return NULL;
        }
        strcpy(result, str);
        return result;
    }

    // Count how many times searchvalue appears in the original string
    int count = 0;
    const char *tmp = str;
    while ((tmp = strstr(tmp, searchvalue)) != NULL) {
        count++;
        tmp += searchvalue_len;  // Move past the last found occurrence
    }

    // Calculate the length of the new string
    int new_len = str_len + count * (newvalue_len - searchvalue_len);

    // Ensure new_len is not negative
    if (new_len < 0) {
        new_len = 0;
    }

    // Allocate enough memory for the new string
    char *result = malloc(new_len + 1);  // +1 for the null terminator
    if (result == NULL) {
        return NULL;  // Return NULL if memory allocation fails
    }

    // Replace all occurrences of searchvalue with newvalue
    const char *current = str;
    char *insert_point = result;
    while (1) {
        const char *p = strstr(current, searchvalue);  // Find the next occurrence
        if (p == NULL) {
            // Copy the rest of the original string if no more occurrences are found
            strcpy(insert_point, current);
            break;
        }

        // Copy the part before the occurrence of searchvalue
        memcpy(insert_point, current, p - current);
        insert_point += p - current;

        // Copy the newvalue in place of searchvalue
        memcpy(insert_point, newvalue, newvalue_len);
        insert_point += newvalue_len;

        // Move the current pointer past the last occurrence of searchvalue
        current = p + searchvalue_len;
    }

    // No need to manually add the null terminator as strcpy and memcpy handle it

    return result;  // Return the newly created string
}


static inline void push(Stack *stack, Position pos) {
    if (stack->top < MAX_STACK - 1) {
        stack->positions[++stack->top] = pos;
    }
}

static inline Position pop(Stack *stack) {
    if (stack->top >= 0) {
        return stack->positions[stack->top--];
    }
    Position invalid = {-1, -1};
    return invalid;  // Invalid position for underflow
}

static inline bool is_empty(Stack *stack) {
    return stack->top == -1;
}

static inline Position check_parentheses(const char *text) {
    
    Stack stack = {.top = -1};
    int line = 1, column = 0;
    Position error_pos = {-1, -1};
    
    for (const char *ptr = text; *ptr != '\0'; ptr++) {
        column++;

        // Handle newlines and reset column count
        if (*ptr == '\n') {
            line++;
            column = 0;
            continue;
        }

        // When we encounter an opening parenthesis
        if (*ptr == '(') {
            Position pos = {line, column};
            push(&stack, pos);
        }
        // When we encounter a closing parenthesis
        else if (*ptr == ')') {
            if (is_empty(&stack)) {
                // No matching opening parenthesis
                error_pos.line = line;
                error_pos.column = column;
                break;
            }
            pop(&stack);  // Pop the matching opening parenthesis
        }
    }

    // If there's an unmatched opening parenthesis left in the stack
    if (!is_empty(&stack)) {
        error_pos = pop(&stack);  // Get the position of the unmatched opening parenthesis
    }

    // Output the result
    if (error_pos.line != -1) {
      //printf("Parentheses error at line %d, column %d\n", error_pos.line, error_pos.column);
    }else {
      //printf("No parentheses errors found.\n");
    }
    
    return error_pos;
}

