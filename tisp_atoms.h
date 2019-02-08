#ifndef _TISP_ATOMS_H_
#define _TISP_ATOMS_H_
#include "tisp_config.h"
	
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum{
  PRE_FREE_ATOMS=0,
  FREE,
  UNINITIALIZED,
  FUNCTION,
  NUMBER,
  STRING
}AtomType;

typedef enum{
  F_UNKNOWN,
  F_UNDEFINED,
	F_SZ,
  F_SZ_SZ,
  F_N_N_N,
	F_SZ_N_N_N,
	F_SZ_N
}FunctionType;

typedef struct Atom{
  AtomType type;
  union{
    struct{
      int32_t int32_value;
      char    sz_value[MAX_ATOM_STR_SIZE];
    };
    struct{
      FunctionType c_func_type;
      char label[MAX_FUN_LABEL_STR_SIZE];
      union{
				void(*c_func_sz)(char* ret, size_t ret_size);
        void(*c_func_sz_sz)(const char* op1, char* ret, size_t ret_size);
        void(*c_func_n_n_n)(int32_t op1, int32_t op2, int32_t* ret);
				void(*c_func_sz_n_n_n)(int32_t op1, int32_t op2, int32_t op3, char* ret, size_t ret_size);
				void(*c_func_sz_n)(int32_t op1, char* ret, size_t ret_size);
        //int32_t(*c_func_2)(int32_t op1, int32_t op2);
        //const char*(*c_func_3)(int32_t op1, int32_t op2);
        //const char*(*c_func_4)(const char* op1, const char* op2);
      };
      struct Atom*  children[MAX_FUN_PARAMS];
      size_t        num_children;
    };
  };

  uint8_t ref_count;

}Atom;


Atom ATOMS[MAX_TREE_ELEMENTS];
size_t available_atoms;


typedef void(*c_func_t)(void);

void init_atoms();
Atom* allocate_atom(AtomType type);
//Atom* allocate_atom_type_number(int32_t value);
//Atom* allocate_atom_type_string(const char* value);
Atom* reallocate_atom(Atom* atom);
void free_atom(Atom* atom);
#ifdef DEBUG
void indent_print(const char* str, size_t indent_level);
void print_ast(Atom* atom, size_t depth);
void print_atom(Atom* atom, size_t indent_level);
void print_atom_stats();
void print_atoms_from_to(size_t from, size_t to);
#else
#define indent_print(str, indent_level) (void)0
#define print_ast(a,b);
#define print_atom(a,b) (void)0
#define print_atom_stats() (void)0
#define print_atoms_from_to(from, to) (void)0
#endif
void tisp_tostring(Atom* atom, char* str);


#endif//_TISP_ATOMS_H_
