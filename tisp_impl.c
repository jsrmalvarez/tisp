#include "tisp_impl.h"
#include <stdio.h>


const char* print_impl(const char* str){
  printf("%s", str);
  return str;
}
