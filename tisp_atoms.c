#include "tisp_atoms.h"
#include <string.h>
#include <stdio.h>

static Atom ATOMS[MAX_TREE_ELEMENTS];
static size_t available_atoms = MAX_TREE_ELEMENTS;
static bool atoms_initialized = false;

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
  Atom* atom = NULL;
  if(type != FREE){
    if(!atoms_initialized){
      init_atoms();
    }

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

  return atom;
}

void free_atom(Atom* atom){
  memset(atom, 0, sizeof(Atom));
  atom->last_children_index = -1;
  atom->type = FREE;

  available_atoms++;
}

void tisp_tostring(Atom* atom, char* str){
  switch(atom->type){
    case FREE:
      printf("FREE");
      break;
    case UNINITIALIZED:
      printf("UNINITIALIZED");
    case STRING:
      printf("%s", atom->sz_value);
      break;
    case NUMBER:
      printf("%d", atom->int32_value);
      break;
    case FUNCTION:
      printf("<<%s>>", atom->label);
      break;
  }
}
