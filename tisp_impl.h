#ifndef _TISP_IMPL_H_
#define _TISP_IMPL_H_

#include "tisp_atoms.h"

typedef struct{
  char* f_label;
  FunctionType f_type;
  void(*c_func)(void);
}F_LUTEntry;

void print_impl(const char* str, char* ret, size_t ret_size);
void println_impl(const char* str, char* ret, size_t ret_size);
void echo_impl(const char* str, char* ret, size_t ret_size);
void sum_impl(int32_t a, int32_t b, int32_t* ret);

static const F_LUTEntry F_LUT[] = {
  {"println", F_SZ_SZ, (void(*)(void))println_impl},
  {"print", F_SZ_SZ, (void(*)(void))print_impl},
  {"echo", F_SZ_SZ, (void(*)(void))echo_impl},
  {"+", F_N_N_N, (void(*)(void))sum_impl}
};

FunctionType get_f_type(char* f_label);

typedef void(*c_func_t)(void);
c_func_t get_c_func(char* f_label);

#endif//_TISP_IMPL_H_
