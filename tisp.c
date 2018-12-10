#include "tisp_interpreter.h"
#include "tisp.h"
#include "tisp_impl.h"
#include <stdio.h>
#include <string.h>

static RuntimeErr error = RUNTIME_ERR_NO_ERR;

RuntimeErr tisp_get_error(){
  return error;
}

void check_param_number(Atom* fun){
  switch(fun->c_func_type){
    case F_SZ_SZ:
      if((fun->last_children_index + 1) != 1){
        error = RUNTIME_ERR_BAD_ARITY;
      }
      break;
    case F_N_N_N:
      if((fun->last_children_index + 1) != 2){
        error = RUNTIME_ERR_BAD_ARITY;
      }
      break;
    default:
      error = RUNTIME_ERR_NOT_IMPLEMENTED;
      return;
      break;
  }
}

void check_param_types(Atom* fun){
  ssize_t n = 0;
  for(; n <= fun->last_children_index; n++){
    Atom* param = fun->children[n];
    if(fun->c_func_type == F_SZ_SZ){
      if(n == 0 && param->type != STRING && param->type != NUMBER){
        error = RUNTIME_ERR_BAD_PARAMETER_TYPE;
        break;
      }
    }
    else if(fun->c_func_type == F_N_N_N){
      if(n == 0 && param->type != NUMBER){
        error = RUNTIME_ERR_BAD_PARAMETER_TYPE;
        break;
      }
      if(n == 1 && param->type != NUMBER){
        error = RUNTIME_ERR_BAD_PARAMETER_TYPE;
        break;
      }
    }
    else{
      error = RUNTIME_ERR_NOT_IMPLEMENTED;
      break;
    }
  }
}

void assign_c_func(Atom* fun){
  switch(fun->c_func_type){
    case F_SZ_SZ:
      {
      c_func_t c_func = get_c_func(fun->label);
      fun->c_func_sz_sz = (void(*)(const char*,char*,size_t))c_func;
      }
      break;
    case F_N_N_N:
      {
      c_func_t c_func = get_c_func(fun->label);
      fun->c_func_n_n_n = (void(*)(int32_t, int32_t, int32_t*))c_func;
      }
      break;
    default:
      error = RUNTIME_ERR_NOT_IMPLEMENTED;
      return;
      break;
  }
}

Atom* call_c_func(Atom* fun){
  Atom* ret_val = NULL;
  switch(fun->c_func_type){
    case F_SZ_SZ:
      {
        const char * parameter;
        // Check for implicit conversion to string
        if(fun->children[0]->type == NUMBER){
          if(fun->children[0]->sz_value[0] == 0){
            // Conversion never made
            sprintf(fun->children[0]->sz_value, "%d", fun->children[0]->int32_value);
          }
        }

        parameter = fun->children[0]->sz_value;
        ret_val = allocate_atom(STRING);
        if(ret_val == NULL){
          error = RUNTIME_ERR_NO_FREE_ATOMS;
        }
        else{
          printf("call_c_func of: %s (type SZ_SZ)\n", fun->label);
          fun->c_func_sz_sz(parameter, ret_val->sz_value, sizeof(ret_val->sz_value));
        }
      }
      break;
    case F_N_N_N:
      {
        ret_val = allocate_atom(NUMBER);
        if(ret_val == NULL){
          error = RUNTIME_ERR_NO_FREE_ATOMS;
        }
        else{
          ret_val->type = NUMBER;
          printf("call_c_func of: %s (type N_N_N)\n", fun->label);
          fun->c_func_n_n_n(fun->children[0]->int32_value,
                            fun->children[1]->int32_value,
                            &ret_val->int32_value);
        }
      }
      break;
    default:
      error = RUNTIME_ERR_NOT_IMPLEMENTED;
      break;
  }

  return ret_val;
}

Atom* tisp_eval(Atom* input){
  
  error = RUNTIME_ERR_NO_ERR;

  Atom* ret_val = NULL;

  switch(input->type){
    case FUNCTION:
      #ifdef DEBUG
      printf("NAME:%s, TYPE:FUNCTION\n", input->label);
      #endif

      if(input->c_func_type == F_UNKNOWN){
        // Never seen this function before.
        // Try to identify the function
        input->c_func_type = get_f_type(input->label);
        if(input->c_func_type == F_UNDEFINED){
          error = RUNTIME_ERR_UNKNOWN_FUN;
          ret_val = NULL;
          break;
        }
        else{
          check_param_number(input);
          if(error != RUNTIME_ERR_NO_ERR){
            ret_val = NULL;
            break;
          }
        }

      }

      ssize_t n = 0;
      for(; n <= input->last_children_index; n++){
        Atom* eval_result = tisp_eval(input->children[n]);
        if(error != RUNTIME_ERR_NO_ERR){
          ret_val = NULL;
          break;
        }

        input->children[n] = eval_result;
      }

      if(error == RUNTIME_ERR_NO_ERR){
        check_param_types(input);
        if(error == RUNTIME_ERR_NO_ERR){
          assign_c_func(input);
          if(error == RUNTIME_ERR_NO_ERR){
            ret_val = call_c_func(input);
          }
          else{
            ret_val = NULL;
          }
        }
        else{
          ret_val = NULL;
        }
      }
      break;

    case NUMBER:
      #ifdef DEBUG
      printf("NAME:%s, TYPE:NUMBER VALUE: %d\n", input->label, input->int32_value);
      #endif
      ret_val = input;
      break;

    case STRING:
      #ifdef DEBUG
      printf("NAME:%s, TYPE:STRING VALUE: %s\n", input->label, input->sz_value);
      #endif
      ret_val = input;
      break;
    default:
      #ifdef DEBUG
      error = RUNTIME_ERR_NOT_IMPLEMENTED;
      #endif
      ret_val = NULL;
      break;

  }

  return ret_val;
}

