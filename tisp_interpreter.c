#include "tisp_interpreter.h"
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <errno.h>
#include <ctype.h>
#include "tisp_atoms.h"
#include "tisp_impl.h"
#ifdef DEBUG
#include <stdio.h>
#endif


//static char output_buffer[256];
//static size_t output_idx = 0;

static uint8_t error = SYNTAX_ERR_NO_ERROR;


#ifndef DEBUG
#define printf(a, ...) (void)0
#endif

#ifndef DEBUG
#define indent_print(str, indent_level) (void)0
#else
void indent_print(const char* str, size_t indent_level){

  const char* pc = str;
  for(size_t n = 0; n < indent_level; n++){
    printf(" ");
  }
  while(*pc != 0){
    printf("%c", *pc);
    if(*pc == '\n' && *(pc+1) != 0){
      for(size_t n = 0; n < indent_level; n++){
        printf(" ");
      }
    }
    pc++;
  }
}
#endif

#ifdef DEBUG
void print_atom(Atom* atom, size_t indent_level){
  indent_print(">>", indent_level);
  printf(atom->label, indent_level);
  printf("<<\n");
}
#endif


#ifdef DEBUG
void print_ast(Atom* atom, size_t depth){
  print_atom(atom, depth);
  ssize_t n = 0;
  for(; n <= atom->last_children_index; n++){
    print_ast(atom->children[n], depth+1);
  }
}
#endif

Atom* parse_atom(const char* atom_start, const char* atom_end, bool is_function){
  Atom* ret_val = allocate_atom(UNINITIALIZED);
  if(ret_val){

    size_t atom_size = atom_end - atom_start;
    if(atom_size + 1 > MAX_ATOM_STR_SIZE){
      error |= SYNTAX_ERR_ATOM_LABEL_TOO_LONG;
      return NULL;
    }
    memcpy(ret_val->label, atom_start, atom_size);
    ret_val->label[atom_size] = 0;
    

    if(is_function){
      ret_val->type = FUNCTION;
      ret_val->c_func_type = F_UNKNOWN;
    }
    else{
      int32_t numerical_value;
      if((numerical_value = strtol(atom_start, NULL, 0)) != 0){ // Base autodetected
        ret_val->type = NUMBER;
        ret_val->int32_value = numerical_value;
        ret_val = reallocate_atom(ret_val);
      }
      else{
        if(errno != 0){
          error |= SYNTAX_ERR_SYNTAX_ERROR;
          return NULL;
        }
        else{
          // strtol returned 0 and no error
          // Test for number
          if(isdigit((int)*atom_start)){
            ret_val->type = NUMBER;
            ret_val->int32_value = 0;
            ret_val = reallocate_atom(ret_val);
          }
          else{
            // strtol returned 0 because no conversion
            // could be performed.
            ret_val->type = STRING;
            // Copy string, the size is already checked:
            memcpy(ret_val->sz_value, atom_start, atom_size);
            ret_val->sz_value[atom_size] = 0;
            ret_val = reallocate_atom(ret_val);
          }
        }
      }
    }
  }
  return ret_val;
}

static bool is_first_on_list;
static Atom* tree_parents[MAX_FUN_RECURSION];
static ssize_t last_tree_parent_index; 

bool process_atom(Atom* new_atom){
  if(!error){
    if(is_first_on_list){

      Atom* tree_parent = tree_parents[last_tree_parent_index];
      if(tree_parent != NULL){
        // Not whole root
        // Insert first_on_list node on parent's children list
        tree_parent->last_children_index++;
        printf("child %ld\n", tree_parent->last_children_index);
        if(tree_parent->last_children_index == MAX_FUN_PARAMS){
          error |= SYNTAX_ERR_TOO_MUCH_PARAMS;
          is_first_on_list = false;
          return false;
        }
        tree_parent->children[tree_parent->last_children_index] = new_atom;
      }

      last_tree_parent_index++;
      printf("root %ld\n", last_tree_parent_index);
      if(last_tree_parent_index == MAX_FUN_RECURSION){
        error |= SYNTAX_ERR_TOO_MUCH_RECURSION;
        is_first_on_list = false;
        return false;
      }
      tree_parents[last_tree_parent_index] = new_atom;

    }
    else{
      Atom* tree_parent = tree_parents[last_tree_parent_index];
      if(tree_parent == NULL){
        error |= SYNTAX_ERR_SYNTAX_ERROR;
        return false;
      }
      else{
        tree_parent->last_children_index++;
        printf("child %ld\n", tree_parent->last_children_index);
        if(tree_parent->last_children_index == MAX_FUN_PARAMS){
          error |= SYNTAX_ERR_TOO_MUCH_PARAMS;
          is_first_on_list = false;
          return false;
        }
        tree_parent->children[tree_parent->last_children_index] = new_atom;
      }
    }

    is_first_on_list = false;
    return true;
  }
  else{
    return false;
  }
}

Atom* tisp_interpreter_read_str(const char* str){
  const char* pc = str;
  bool reading_atom = false;
  const char* atom_start = NULL;
  const char* atom_end = NULL;
  size_t list_level = 0;
  bool create_new_atom = false;

  error = SYNTAX_ERR_NO_ERROR;

  is_first_on_list = false;
  for(size_t n = 0; n < MAX_FUN_RECURSION; n++){
    tree_parents[n] = NULL;
  }
  last_tree_parent_index = -1; 

#define TISP_MARK_ATOM_START() reading_atom = true;  atom_start = pc

#define TISP_MARK_ATOM_END() \
  reading_atom = false; \
  atom_end   = pc; \
  Atom* new_atom = parse_atom(atom_start, atom_end, is_first_on_list); \
  if(new_atom == NULL){ \
    error |= SYNTAX_ERR_TOO_MUCH_ATOMS; \
  } \
  else{ \
    print_atom(new_atom, list_level); \
    process_atom(new_atom); \
  }

  printf("ORIGINAL INPUT:\n>>%s<<\n", str);

  while(*pc != 0){
    char c = *pc;

    if(c == '('){
      if(reading_atom){
        TISP_MARK_ATOM_END();
      }
      indent_print("LIST_BEGIN\n", list_level);
      list_level++;
      is_first_on_list = true;
      if(list_level == 0){
        error |= SYNTAX_ERR_UNBALANCED_PARENTHESES;
        break;
      }
    }
    else if(c == ')'){
      if(reading_atom){
        TISP_MARK_ATOM_END();
      }
      if(list_level == 0){
        error |= SYNTAX_ERR_UNBALANCED_PARENTHESES;
      }
      else{
        list_level--;
      }

      last_tree_parent_index--;

      indent_print("LIST_END\n", list_level);
    }
    else{
      if(c != ' ' && c != '\t' && c != '\n' && c != '\r'){
        if(!reading_atom){
          TISP_MARK_ATOM_START();
        }
      }
      else{
        if(reading_atom){
          TISP_MARK_ATOM_END();
        }
      }
    }

    if(create_new_atom){
      create_new_atom = false;
    }

    pc++;
  }


  if(error == SYNTAX_ERR_NO_ERROR){
    if(reading_atom){
      TISP_MARK_ATOM_END();
    }

    if(list_level != 0){
      error |= SYNTAX_ERR_UNBALANCED_PARENTHESES;
    }
  }

  if(error == SYNTAX_ERR_NO_ERROR){
    Atom* root = tree_parents[0];
    if(root != NULL){
      printf("\nAST:\n");
      print_ast(root, 0);
    }

    return root;
  }
  else{
    return NULL;
  }

}

/*const char* tisp_interpreter_get_output_str(){
  return output_buffer;
}*/

uint8_t tisp_interpreter_get_error(){
  return error;
}
