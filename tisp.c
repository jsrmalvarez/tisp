#include "tisp_interpreter.h"
#include "tisp.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#if (MAX_FUN_LABEL_STR_SIZE > MAX_ATOM_STR_SIZE)
#error MAX_FUN_LABEL_STR_SIZE > MAX_ATOM_STR_SIZE
#else
#endif

// Must be implemeted by user
FunctionType get_f_type(const char* f_label); // Returns function type from function label (which is F_UNDEFINED if the function doesn't exist)
c_func_t get_c_func(const char* f_label); // Returns a pointer to function from function label (or NULL if the function doesn't exist)

static RuntimeErr error = RUNTIME_ERR_NO_ERR;

RuntimeErr tisp_get_error(){
  return error;
}

void check_param_number(Atom* fun){
  switch(fun->c_func_type){
		case F_SZ:
			if((fun->num_children) != 0){
        error = RUNTIME_ERR_BAD_ARITY;
      }
			break;
    case F_SZ_SZ:
      if((fun->num_children) != 1){
        error = RUNTIME_ERR_BAD_ARITY;
      }
      break;
    case F_SZ_N:
      if((fun->num_children) != 1){
        error = RUNTIME_ERR_BAD_ARITY;
      }
      break;			
    case F_N_N_N:
      if((fun->num_children) != 2){
        error = RUNTIME_ERR_BAD_ARITY;
      }
      break;
		case F_SZ_N_N_N:
      if((fun->num_children) != 3){
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
  for(size_t n = 0; n < fun->num_children; n++){
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
		else if(fun->c_func_type == F_SZ_N_N_N){
      if(n == 0 && param->type != NUMBER){
        error = RUNTIME_ERR_BAD_PARAMETER_TYPE;
        break;
      }
      if(n == 1 && param->type != NUMBER){
        error = RUNTIME_ERR_BAD_PARAMETER_TYPE;
        break;
      }
			if(n == 2 && param->type != NUMBER){
        error = RUNTIME_ERR_BAD_PARAMETER_TYPE;
        break;
      }
    }
		else if(fun->c_func_type == F_SZ_N){
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
		case F_SZ:
      {
      c_func_t c_func = get_c_func(fun->label);
      fun->c_func_sz = (void(*)(char*, size_t))c_func;
      }
      break;
    case F_SZ_SZ:
      {
      c_func_t c_func = get_c_func(fun->label);
      fun->c_func_sz_sz = (void(*)(const char*, char*, size_t))c_func;
      }
      break;
    case F_N_N_N:
      {
      c_func_t c_func = get_c_func(fun->label);
      fun->c_func_n_n_n = (void(*)(int32_t, int32_t, int32_t*))c_func;
      }
      break;
		case F_SZ_N_N_N:
			{
      c_func_t c_func = get_c_func(fun->label);
      fun->c_func_sz_n_n_n = (void(*)(int32_t, int32_t, int32_t, char*, size_t))c_func;
      }
			break;
		case F_SZ_N:
			{
      c_func_t c_func = get_c_func(fun->label);
      fun->c_func_sz_n = (void(*)(int32_t, char*, size_t))c_func;
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
		case F_SZ:
      {
				ret_val = allocate_atom(STRING);
        if(ret_val == NULL){
          error = RUNTIME_ERR_NO_FREE_ATOMS;
        }
        else{
          SPAM(("call_c_func of: %s (type SZ)\n", fun->label));
          fun->c_func_sz(ret_val->sz_value, sizeof(ret_val->sz_value));
          ret_val = reallocate_atom(ret_val);
        }	
			}
			break;
    case F_SZ_SZ:
      {
        const char * parameter;
        // Check for implicit conversion to string
        if(fun->children[0]->type == NUMBER){
          if(fun->children[0]->sz_value[0] == 0){
            // Conversion never made
            sprintf(fun->children[0]->sz_value, "%"PRIi32"", fun->children[0]->int32_value);
          }
        }

        parameter = fun->children[0]->sz_value;
        ret_val = allocate_atom(STRING);
        if(ret_val == NULL){
          error = RUNTIME_ERR_NO_FREE_ATOMS;
        }
        else{
          SPAM(("call_c_func of: %s (type SZ_SZ)\n", fun->label));
          fun->c_func_sz_sz(parameter, ret_val->sz_value, sizeof(ret_val->sz_value));
          ret_val = reallocate_atom(ret_val);
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
          //ret_val->type = NUMBER;
          SPAM(("call_c_func of: %s (type N_N_N)\n", fun->label));
          fun->c_func_n_n_n(fun->children[0]->int32_value,
                            fun->children[1]->int32_value,
                            &ret_val->int32_value);
          ret_val = reallocate_atom(ret_val);
        }
      }
      break;
		case F_SZ_N_N_N:
			{
				ret_val = allocate_atom(STRING);
        if(ret_val == NULL){
          error = RUNTIME_ERR_NO_FREE_ATOMS;
        }
        else{
          //ret_val->type = STRING;
          SPAM(("call_c_func of: %s (type SZ_N_N_N)\n", fun->label));
          fun->c_func_sz_n_n_n(fun->children[0]->int32_value,
                            fun->children[1]->int32_value,
														fun->children[2]->int32_value,
                            ret_val->sz_value, sizeof(ret_val->sz_value));
          ret_val = reallocate_atom(ret_val);
        }
			}
			break;
			case F_SZ_N:
			{
				ret_val = allocate_atom(STRING);
        if(ret_val == NULL){
          error = RUNTIME_ERR_NO_FREE_ATOMS;
        }
        else{
          //ret_val->type = STRING;
          SPAM(("call_c_func of: %s (type SZ_N)\n", fun->label));
          fun->c_func_sz_n(fun->children[0]->int32_value,                            
                            ret_val->sz_value, sizeof(ret_val->sz_value));
          ret_val = reallocate_atom(ret_val);
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
      SPAM(("NAME:%s, TYPE:FUNCTION\n", input->label));
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

      for(size_t n = 0; n < input->num_children; n++){
        Atom* eval_result = tisp_eval(input->children[n]);
        if(error != RUNTIME_ERR_NO_ERR){
          ret_val = NULL;
          break;
        }

        if(input->children[n]->type == FUNCTION){
          // FUNCTIONS eventually evaluate to STRING or NUMBER
          // Substitute FUNCTION child for its result atom
          // and discard FUNCTION child;
          SPAM(("-- PRE SUBSTITUTE\n"));
          print_ast(input, 0);

          Atom* evaluated_function_child = input->children[n];
          input->children[n] = eval_result;
          input->children[n]->ref_count++;
          evaluated_function_child->ref_count--;
          free_atom(evaluated_function_child);

          SPAM(("-- POST SUBSTITUTE\n"));
          print_ast(input, 0);
        }
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
      ret_val = input;
      break;

    case STRING:
      ret_val = input;
      break;
    default:
      error = RUNTIME_ERR_NOT_IMPLEMENTED;
      ret_val = NULL;
      break;

  }

  return ret_val;
}

