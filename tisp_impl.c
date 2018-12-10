#include "tisp_impl.h"
#include <stdio.h>
#include <string.h>

#include "tisp_impl.h"

FunctionType get_f_type(char* f_label){
  FunctionType ret_val = F_UNDEFINED;
  for(size_t n = 0; n < sizeof(F_LUT)/sizeof(F_LUT[0]); n++){
    if(memcmp(f_label, F_LUT[n].f_label, strlen(F_LUT[n].f_label)) == 0){
      ret_val = F_LUT[n].f_type;
      break;
    }
  }


  return ret_val; 
}

c_func_t get_c_func(char* f_label){
  c_func_t ret_val = NULL;
  for(size_t n = 0; n < sizeof(F_LUT)/sizeof(F_LUT[0]); n++){
    if(memcmp(f_label, F_LUT[n].f_label, strlen(F_LUT[n].f_label)) == 0){
      ret_val = F_LUT[n].c_func;
      break;
    }
  }

  return ret_val; 
}

void print_impl(const char* str, char* ret, size_t ret_size){  
  echo_impl(str, ret, ret_size);
  printf("%s", str);
}

void println_impl(const char* str, char* ret, size_t ret_size){
  print_impl(str, ret, ret_size);
  printf("\n");
}

void echo_impl(const char* str, char* ret, size_t ret_size){
  strncpy(ret, str, ret_size);
  ret[ret_size - 1] = '\0';
}

void sum_impl(int32_t a, int32_t b, int32_t* ret){
  *ret = a + b;
}
