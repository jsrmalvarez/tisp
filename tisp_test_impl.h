#ifndef _TISP_TEST_IMPL_H_
#define _TISP_TEST_IMPL_H_

#include "tisp_atoms.h"
		
typedef struct{
  char* f_label;
  FunctionType f_type;
  void(*c_func)(void);
}F_LUTEntry;

/*void print_impl(const char* str, char* ret, size_t ret_size);
void println_impl(const char* str, char* ret, size_t ret_size);
void echo_impl(const char* str, char* ret, size_t ret_size);*/
void help_impl(char* ret, size_t ret_size);
void echo_off_impl(char* et, size_t ret_size);
void echo_on_impl(char* ret, size_t ret_size);
//void sum_impl(int32_t a, int32_t b, int32_t* ret);
void set_time_impl(int32_t h, int32_t m, int32_t s, char* ret, size_t ret_size);
void set_date_impl(int32_t h, int32_t m, int32_t s, char* ret, size_t ret_size);
void get_time_impl(char* ret, size_t ret_size);
void get_date_impl(char* ret, size_t ret_size);
//void atoms_impl(int32_t from, int32_t to, char* ret, size_t ret_size);

static const F_LUTEntry F_LUT[] = {
  /*{"println", F_SZ_SZ, (void(*)(void))println_impl}
  ,{"print", F_SZ_SZ, (void(*)(void))print_impl}
  ,{"echo", F_SZ_SZ, (void(*)(void))echo_impl}*/
	 {"help", F_SZ, (void(*)(void))help_impl}
	,{"echo_off", F_SZ, (void(*)(void))echo_off_impl}
	,{"echo_on", F_SZ, (void(*)(void))echo_on_impl}
//  ,{"+", F_N_N_N, (void(*)(void))sum_impl}
	,{"set_time", F_SZ_N_N_N, (void(*)(void))set_time_impl}
	,{"set_date", F_SZ_N_N_N, (void(*)(void))set_date_impl}	
	,{"get_time", F_SZ, (void(*)(void))get_time_impl}	
	,{"get_date", F_SZ, (void(*)(void))get_date_impl}	
//,{"atoms", F_SZ, (void(*)(void))atoms_impl}
};


#endif//_TISP_TEST_IMPL_H_
