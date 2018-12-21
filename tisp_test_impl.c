#include <stdio.h>
#include <string.h>

#include "tisp_test_impl.h"
#include "tisp_atoms.h"
#include "app_base.h"
#include <simple_terminal.h>

#include "process_user_requests_task.h" // TODO No debería tener que incluir esto

FunctionType get_f_type(char* f_label){
  FunctionType ret_val = F_UNDEFINED;
  for(size_t n = 0; n < sizeof(F_LUT)/sizeof(F_LUT[0]); n++){
    if(memcmp(f_label, F_LUT[n].f_label, strlen(f_label)) == 0){
      ret_val = F_LUT[n].f_type;
      break;
    }
  }


  return ret_val; 
}

c_func_t get_c_func(char* f_label){
  c_func_t ret_val = NULL;
  for(size_t n = 0; n < sizeof(F_LUT)/sizeof(F_LUT[0]); n++){
    if(memcmp(f_label, F_LUT[n].f_label, strlen(f_label)) == 0){
      ret_val = F_LUT[n].c_func;
      break;
    }
  }

  return ret_val; 
}

/*
void print_impl(const char* str, char* ret, size_t ret_size){  
  echo_impl(str, ret, ret_size);
  printf("%s", str);
}

void println_impl(const char* str, char* ret, size_t ret_size){
  print_impl(str, ret, ret_size);
  printf("\n");
}*/

/*void echo_impl(const char* str, char* ret, size_t ret_size){
  strncpy(ret, str, ret_size);
  ret[ret_size - 1] = '\0';
}*/

/*void sum_impl(int32_t a, int32_t b, int32_t* ret){
  *ret = a + b;
}*/

static const char* OK_STR = "OK";
static const char* KO_STR = "KO";

void set_time_impl(int32_t h, int32_t m, int32_t s, char* ret, size_t ret_size){
	bool ok = app_set_time(h,m,s, true, 5000);
	strncpy(ret, ok ? OK_STR : KO_STR, ret_size);
  ret[ret_size - 1] = '\0';
}

void set_date_impl(int32_t y, int32_t m, int32_t d, char* ret, size_t ret_size){
	bool ok = app_set_date(y,m,d, true, 5000);
	strncpy(ret, ok ? OK_STR : KO_STR, ret_size);
  ret[ret_size - 1] = '\0';
}

void get_time_impl(char* ret, size_t ret_size){
	bool ok = app_get_time(ret, ret_size, true, 5000);
	if(!ok){
		strncpy(ret, KO_STR, ret_size);
	}
  ret[ret_size - 1] = '\0';
}

void get_date_impl(char* ret, size_t ret_size){
	bool ok = app_get_date(ret, ret_size, true, 5000);
	if(!ok){
		strncpy(ret, KO_STR, ret_size);
	}
  ret[ret_size - 1] = '\0';
}

void echo_off_impl(char* ret, size_t ret_size){
	Terminal t = app_get_terminal();
	terminal_set_char_echo(t, false);
	strncpy(ret, OK_STR, ret_size);
}

void echo_on_impl(char* ret, size_t ret_size){
	Terminal t = app_get_terminal();
	terminal_set_char_echo(t, true);
	strncpy(ret, OK_STR, ret_size);
}

void help_impl(char* ret, size_t ret_size){	
	for(size_t n = 0; n < sizeof(F_LUT)/sizeof(F_LUT[0]); n++){
		terminal_send_chars(F_LUT[n].f_label, strlen(F_LUT[n].f_label));
		terminal_send_char('\n');		
	}
	strncpy(ret, OK_STR, ret_size);
}
/*
void atoms_impl(int32_t from, int32_t to, char* ret, size_t ret_size){
  if(from == -1){
    // From the beginning
    from = 0;
  }

  if(to == -1){
    // to the end
    to = MAX_TREE_ELEMENTS;
  }


  if(from < 0 || to < 0 || ret == NULL){
    size_t N = strlen(ERROR);
    if (strlen(ERROR) > ret_size){
      N = ret_size;
    }

    if(N > 0){
      strncpy(ret, ERROR, N);
      ret[N - 1] = '\0';
    }
  }
  else{
    for(size_t n = from; n < to; n++){
      char str[MAX_ATOM_STR_SIZE];
      tisp_tostring(&ATOMS[n], str);
      printf("ATOMS[%03lu] 0x%08lX (%03u): %s\n", n, (uintptr_t)&ATOMS[n], ATOMS[n].ref_count, str);
    }
  }
}
*/
