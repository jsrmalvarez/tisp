#include "tisp_atoms.h"
#include <string.h>
#include <stdio.h>


#ifndef DEBUG
#define printf(a, ...) (void)0
#endif


static Atom ATOMS[MAX_TREE_ELEMENTS];
static size_t available_atoms = 0;

void init_atom(Atom* atom){  
  memset(atom, 0, sizeof(Atom));
  atom->num_children = 0;
  atom->type = PRE_FREE_ATOMS;
  atom->ref_count = 0;
}

void init_atoms(){
  for(size_t n = 0; n < sizeof(ATOMS)/sizeof(ATOMS[0]); n++){
    free_atom(&ATOMS[n]);
  }
}

#ifdef DEBUG
void print_atom_stats(){
  printf("Available atoms: %ld / %lu\n", available_atoms, sizeof(ATOMS)/sizeof(ATOMS[0]));
}
#endif

Atom* allocate_atom(AtomType type){
  static bool atoms_initialized = false;
  if(!atoms_initialized){
    init_atoms();
    atoms_initialized = true;
  }

  Atom* atom = NULL;
  if(available_atoms > 0 && type != FREE){

    // Search for free atoms
    for(size_t n = 0; n < sizeof(ATOMS)/sizeof(ATOMS[0]); n++){
      if(ATOMS[n].type == FREE){
        atom = &ATOMS[n];
        atom->type = type;
        available_atoms--;
        break;
      }
    }
  }
  else{
    // TODO: error 
  }

  print_atom_stats();

  return atom;
}

Atom* reallocate_atom(Atom* atom){
  Atom* ret_val = atom;
  for(size_t n = 0; n < sizeof(ATOMS)/sizeof(ATOMS[0]); n++){
    if(&ATOMS[n] != atom){
      // Not the same atom
      if(ATOMS[n].type == STRING && atom->type == STRING){
        if(strncmp(ATOMS[n].sz_value, atom->sz_value, sizeof(atom->sz_value)) == 0){
          // Should never free an atom with ref_count > 0
          // Must check if the ret_val atom can withstand more refs.
          if(atom->ref_count == 0 && ret_val->ref_count < UINT8_MAX){
            free_atom(atom);
            ret_val = &ATOMS[n];
            printf("Found equivalent STRING atom! Reallocated. Saved 1 atom in memory.\n");
            break;
          }
        }
      }
      else if(ATOMS[n].type == NUMBER && atom->type == NUMBER){
        if(ATOMS[n].int32_value == atom->int32_value){
          // Should never free an atom with ref_count > 0
          // Must check if the ret_val atom can withstand more refs.
          if(atom->ref_count == 0 && ret_val->ref_count < UINT8_MAX){
            free_atom(atom);
            ret_val = &ATOMS[n];
            printf("Found equivalent NUMBER atom! Reallocated. Saved 1 atom in memory.\n");
            break;
          }
        }
      }
    }
  }
  return ret_val;
}

void free_atom(Atom* atom){
#ifdef DEBUG    
    char str[MAX_ATOM_STR_SIZE];
    tisp_tostring(atom, str);
#endif    
  if(atom->type != FREE && (atom->ref_count == 0 || available_atoms == MAX_TREE_ELEMENTS - 1)){
    if(atom->type == FUNCTION){
      for(size_t n = 0; n < atom->num_children; n++){
        atom->children[n]->ref_count--;
        free_atom(atom->children[n]);
      }
    }
#ifdef DEBUG    
    printf("free_atom(%s) 0x%08lX: ", str, (uintptr_t)atom);
#endif    
    init_atom(atom);
    atom->type = FREE;
    available_atoms++;
  }
  else{
#ifdef DEBUG    
    if(atom->type == FREE){
      printf("free_atom(%s) 0x%08lX ALREADY FREE ", str, (uintptr_t)atom);
    }
    else if(atom->ref_count != 0){
      printf("free_atom(%s) 0x%08lX REMAINING REFS ref_count: %u ", str, (uintptr_t)atom, atom->ref_count);
    }
#endif    
  }
  print_atom_stats();
}

void tisp_tostring(Atom* atom, char* str){
  switch(atom->type){
    case PRE_FREE_ATOMS:
      sprintf(str, "PRE_FREE_ATOMS");
      break;
    case FREE:
      sprintf(str, "FREE");
      break;
    case UNINITIALIZED:
      sprintf(str, "UNINITIALIZED");
    case STRING:
      sprintf(str, "%s", atom->sz_value);
      break;
    case NUMBER:
      sprintf(str, "%d", atom->int32_value);
      break;
    case FUNCTION:
      sprintf(str, "(%s)", atom->label);
      break;
  }
}

#ifdef DEBUG
void print_atoms_from_to(size_t from, size_t to){
  for(size_t n = from; n < to; n++){
    char str[MAX_ATOM_STR_SIZE];
    tisp_tostring(&ATOMS[n], str);
    printf("ATOMS[%03lu] 0x%08lX (%03u): %s\n", n, (uintptr_t)&ATOMS[n], ATOMS[n].ref_count, str);
  }
}
#endif

#ifdef DEBUG
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
  indent_print(" ", indent_level);
  char str[MAX_ATOM_STR_SIZE];
  tisp_tostring(atom, str);
  printf("%s", str);
  printf(" : 0x%08lX ref_count: %u\n", (uintptr_t)atom, atom->ref_count);
}
#endif


#ifdef DEBUG
void print_ast(Atom* atom, size_t depth){
  print_atom(atom, depth);
  for(size_t n = 0; n < atom->num_children; n++){
    print_ast(atom->children[n], depth+1);
  }
}
#endif
