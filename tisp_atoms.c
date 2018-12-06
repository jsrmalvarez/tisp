#include "tisp_atoms.h"
#include <string.h>
#include <stdio.h>

static Atom ATOMS[MAX_TREE_ELEMENTS];
static Atom* next_atom = ATOMS;
static size_t available_atoms = MAX_TREE_ELEMENTS;

Atom* allocate_atom(){
  Atom* atom = NULL;
  if(available_atoms){
    atom = next_atom;
    next_atom++;
    available_atoms--;
  }

  atom->sz_value[0] = 0;
  atom->free = true;

  return atom;
}

Atom* allocate_number_atom(int32_t value){
  Atom* atom = allocate_atom();
  if(atom){
    atom->type = NUMBER;
    atom->int32_value = value;
  }

  return atom;
}

Atom* allocate_string_atom(const char *value){
  Atom* atom = allocate_atom();
  if(atom){
    atom->type = STRING;
    size_t value_size;
    value_size = strlen(value);
    if(value_size > MAX_ATOM_STR_SIZE - 1){
      value_size = MAX_ATOM_STR_SIZE - 1;
    }
    memcpy(atom->sz_value, value, value_size);
    atom->sz_value[value_size + 1] = 0;
  }

  return atom;
}

void tisp_tostring(Atom* atom, char* str){
  printf("\n");
  switch(atom->type){
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
  printf("\n");
}
