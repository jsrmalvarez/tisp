#include "tisp_atoms.h"
#include <string.h>
#include <stdio.h>

static Atom ATOMS[MAX_TREE_ELEMENTS];
static size_t available_atoms = 0;

void init_atom(Atom* atom){  
  memset(atom, 0, sizeof(Atom));
  atom->last_children_index = -1;
  atom->type = PRE_INIT_ATOMS;
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
          free_atom(atom);
          ret_val = &ATOMS[n];
          printf("Found equivalent STRING atom! Reallocated. Saved 1 atom in memory.\n");
          break;
        }
      }
      else if(ATOMS[n].type == NUMBER && atom->type == NUMBER){
        if(ATOMS[n].int32_value == atom->int32_value){
          free_atom(atom);
          ret_val = &ATOMS[n];
          printf("Found equivalent NUMBER atom! Reallocated. Saved 1 atom in memory.\n");
          break;
        }
      }
    }
  }
  return ret_val;
}

void free_atom(Atom* atom){
  if(atom->type != FREE){
    if(atom->type == FUNCTION){
      for(ssize_t n = 0; n <= atom->last_children_index; n++){
        free_atom((Atom*)&atom->children[n]);
      }
    }
    init_atom(atom);
    atom->type = FREE;
    available_atoms++;
  }
  printf("FREE: ");
  print_atom_stats();
}

void tisp_tostring(Atom* atom, char* str){
  switch(atom->type){
    case PRE_INIT_ATOMS:
      printf("PRE_INIT_ATOMS");
      break;
    case FREE:
      printf("FREE");
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
      sprintf(str, "<<%s>>", atom->label);
      break;
  }
}
