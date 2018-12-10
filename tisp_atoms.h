#ifndef _TISP_ATOMS_H_
#define _TISP_ATOMS_H_

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum{
  FREE,
  UNINITIALIZED,
  FUNCTION,
  NUMBER,
  STRING
}AtomType;

typedef enum{
  F_UNKNOWN,
  F_UNDEFINED,
  F_SZ_SZ,
  F_N_N_N
}FunctionType;


#define MAX_ATOM_STR_SIZE 16
#define MAX_FUN_PARAMS 4
#define MAX_FUN_RECURSION 4
#define MAX_TREE_ELEMENTS 256//MAX_FUN_PARAMS^MAX_FUN_RECURSION

typedef struct Atom{
  AtomType type;
  union{
    struct{
      int32_t int32_value;
      char    sz_value[MAX_ATOM_STR_SIZE];
    };
    struct{
      FunctionType c_func_type;
      union{
        void(*c_func_sz_sz)(const char* op1, char* ret, size_t ret_size);
        void(*c_func_n_n_n)(int32_t op1, int32_t op2, int32_t* ret);
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

void init_atoms();
Atom* allocate_atom(AtomType type);
void free_atom(Atom* atom);
#ifdef DEBUG
void print_atom_stats();
#else
#define print_atom_stats() (void)0
#endif
void tisp_tostring(Atom* atom, char* str);


#endif//_TISP_ATOMS_H_
