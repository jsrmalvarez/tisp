#ifndef _TISP_INTERPRETER_H_
#define _TISP_INTERPRETER_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdint.h>
#include <stdlib.h>

#define SYNTAX_ERR_NO_ERROR                0
#define SYNTAX_ERR_UNBALANCED_PARENTHESES (1 << 0)
#define SYNTAX_ERR_TOO_MUCH_ATOMS         (1 << 1)
#define SYNTAX_ERR_TOO_MUCH_PARAMS        (1 << 2)
#define SYNTAX_ERR_TOO_MUCH_RECURSION     (1 << 3)
#define SYNTAX_ERR_ATOM_LABEL_TOO_LONG    (1 << 4)
#define SYNTAX_ERR_SYNTAX_ERROR           (1 << 5)


typedef enum{
  FUNCTION,
  NUMBER,
  STRING
}AtomType;

typedef enum{
  F_UNKNOWN,
  F_UNDEFINED,
  F_SZ_SZ
}FunctionType;


#define MAX_ATOM_STR_SIZE 16
#define MAX_FUN_PARAMS 4
#define MAX_FUN_RECURSION 4
#define MAX_TREE_ELEMENTS 256//MAX_FUN_PARAMS^MAX_FUN_RECURSION

typedef struct Atom{

// Type and value according to type.
// Value is obtained with eval(Atom).
// * If the atom is a function, the value is calculated as the return 
//   of the corresponding c_func. The arguments to the c_func are the
//   values of this atom's children.
// * If the atom is a number, the value is the number
  AtomType type;
  union{
    int32_t int32_value;
    char    sz_value[MAX_ATOM_STR_SIZE];
    struct{
      FunctionType c_func_type;
      union{
        const char*(*c_func_sz_sz)(const char* op1);
        //int32_t(*c_func_2)(int32_t op1, int32_t op2);
        //const char*(*c_func_3)(int32_t op1, int32_t op2);
        //const char*(*c_func_4)(const char* op1, const char* op2);
      };
    };
  };

  char      label[MAX_ATOM_STR_SIZE];

  struct Atom*   children[MAX_FUN_PARAMS];
  ssize_t        last_children_index;

}Atom;
Atom* tisp_interpreter_read_str(const char* str);
//const char* tisp_interpreter_get_output_str();
uint8_t tisp_interpreter_get_error();
#ifndef DEBUG
#define print_atom(a,b) (void)0
#else
void print_atom(Atom* atom, size_t indent_level);
#endif

#ifndef DEBUG
#define print_ast(a,b);
#else
void print_ast(Atom* atom, size_t depth);
#endif

#endif//_TISP_INTERPRETER_H_
