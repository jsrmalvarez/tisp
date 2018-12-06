
Atom* allocate_string_atom(const char *ret){
  Atom ret_atom;
  ret_atom.type = STRING;
  size_t ret_size;
  ret_size = strlen(ret);
  if(ret_size > MAX_ATOM_STR_SIZE - 1){
    ret_size = MAX_ATOM_STR_SIZE - 1;
  }
  memcpy(ret_atom.sz_value, ret, ret_size);
  ret_atom.sz_value[ret_size + 1] = 0;
}
