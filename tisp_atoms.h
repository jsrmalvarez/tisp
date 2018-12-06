#ifndef _TISP_ATOMS_H_
#define _TISP_ATOMS_H_

#include <stdint.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

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
  bool free;
  AtomType type;
  union{
    struct{
      int32_t int32_value;
      char    sz_value[MAX_ATOM_STR_SIZE];
    };
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

Atom* allocate_atom();
Atom* allocate_number_atom(int32_t value);
Atom* allocate_string_atom(const char *value);
#define allocate_function_atom() allocate_atom()


#endif//_TISP_ATOMS_H_
