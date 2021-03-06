#include <stdio.h>
#include <string.h>
#include "tisp.h"

uint8_t print_interpreter_error(){
	uint8_t error = tisp_interpreter_get_error();
	if(error){
		printf("\nERROR: ");
    printf("0x%02x\n", error);

		if(error & SYNTAX_ERR_SYNTAX_ERROR){
			printf("- Syntax error.\n");
		}

		if(error & SYNTAX_ERR_ATOM_LABEL_TOO_LONG){
			printf("- Label too long.\n");
		}

		if(error & SYNTAX_ERR_TOO_MUCH_ATOMS){
			printf("- Too much atoms.\n");
		}

		if(error & SYNTAX_ERR_TOO_MUCH_PARAMS){
			printf("- Too much parameters.\n");
		}

		if(error & SYNTAX_ERR_TOO_MUCH_RECURSION){
			printf("- Too much recursion.\n");
		}

		if(error & SYNTAX_ERR_UNBALANCED_PARENTHESES){
			printf("- Unbalanced parentheses.\n");
		}
	}

  return error != SYNTAX_ERR_NO_ERROR;
}

uint8_t print_runtime_error(){
	RuntimeErr error = tisp_get_error();
  switch(error){
    case RUNTIME_ERR_NO_ERR:
      break;
    case RUNTIME_ERR_UNKNOWN_FUN:
      printf("- Unknown function\n");
      break;
    case RUNTIME_ERR_NOT_IMPLEMENTED:
      printf("- Not implemented\n");
      break;
    case RUNTIME_ERR_BAD_ARITY:
      printf("- Bad arity\n");
      break;
    case RUNTIME_ERR_BAD_PARAMETER_TYPE:
      printf("- Bad parameter type\n");
      break;
    case RUNTIME_ERR_NO_FREE_ATOMS:
      printf("- No free atoms\n");
      break;
  }

  return error != RUNTIME_ERR_NO_ERR;
}

int main(){
  //tisp_interpreter_read_str("		(  ( k f KFC ) 8 42 ( f((((((((_uP g)))))))) ) )");
  //tisp_interpreter_read_str("(fun a b\n\t(if (= a 0) 1 3) (* 8 9)  )");
  //tisp_interpreter_read_str("cuarenta_y_dos ) ");
  //tisp_interpreter_read_str("( f a b c d )");
  //tisp_interpreter_read_str("(fun a b\n\t(if (= a 0) 1 3) (* 8 9 (- 2 3 (l 2 3 4 ))) )");
  //Atom* atom = tisp_interpreter_read_str("(print 1)");

  //Atom* atom = tisp_interpreter_read_str("(fun a b\n\t(if (= a 0) 1 3) (* 8 9)  )");
  //printf("\neval\n");
  //Atom* output = eval(atom);
  //printf("%s", tisp_interpreter_get_output_str());


  // REPL
  char line[1024];
  // Loop Read
  while(fgets(line, sizeof(line), stdin)){
    Atom a;
    printf("----> %lu\n", sizeof(a));
    printf("----> %lu\n", sizeof(a.c_func_type));
    printf("----> %lu\n", sizeof(a.label));
    printf("----> %lu\n", sizeof(a.c_func_sz_sz));
    printf("----> %lu\n", sizeof(a.children));
    printf("----> %lu\n", sizeof(a.num_children));

    Atom* input = tisp_interpreter_read_str(line);
    if(!print_interpreter_error()){
      if(input != NULL){
        printf("- Pre eval\n");
        print_atoms_from_to(0,16);
        // Eval
        Atom* result = tisp_eval(input);
        // Print
        if(!print_runtime_error()){
          if(result != NULL){
            char result_str[MAX_ATOM_STR_SIZE];
            tisp_tostring(result, result_str);
            printf("%s\n", result_str);

            printf("- Post eval, pre free input\n");
            print_atoms_from_to(0,16);

            free_atom(input);
            printf("- Post free input, pre free result\n");
            print_atoms_from_to(0,16);
            

            /*printf("\n\n");
            tisp_tostring(result, result_str);
            printf("%s\n", result_str);*/

            free_atom(result);
            printf("- Post free result\n");
            print_atoms_from_to(0,16);
          }
        }
      }
    }
  }
}

