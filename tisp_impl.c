#include "tisp_impl.h"
#include <stdio.h>
#include <string.h>

#include "tisp_impl.h"

ssize_t last_LUT_index = -1;

ssize_t get_last_LUT_index(){
  return last_LUT_index;
}

FunctionType get_f_type(char* f_label){
  FunctionType ret_val = F_UNDEFINED;
  last_LUT_index = -1;
  for(size_t n = 0; n < sizeof(F_LUT)/sizeof(F_LUT[0]); n++){
    if(memcmp(f_label, F_LUT[n].f_label, strlen(F_LUT[n].f_label)) == 0){
      ret_val = F_LUT[n].f_type;
      last_LUT_index = n;
      break;
    }
  }

  if(ret_val == F_UNDEFINED){
    last_LUT_index = -1;
  }

  return ret_val; 
}

void strcpy_max_ensure_zero(const char* str, char* ret, size_t ret_size){
  size_t str_size = strlen(str);
  if(str_size > ret_size - 1){
    str_size = ret_size - 1;
  }
  memcpy(ret, str, str_size);
  ret[str_size + 1] = 0;
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
