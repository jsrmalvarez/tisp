#ifndef _TISP_INTERPRETER_H_
#define _TISP_INTERPRETER_H_

#include "tisp_atoms.h"

#define SYNTAX_ERR_NO_ERROR                0
#define SYNTAX_ERR_UNBALANCED_PARENTHESES (1 << 0)
#define SYNTAX_ERR_TOO_MUCH_ATOMS         (1 << 1)
#define SYNTAX_ERR_TOO_MUCH_PARAMS        (1 << 2)
#define SYNTAX_ERR_TOO_MUCH_RECURSION     (1 << 3)
#define SYNTAX_ERR_ATOM_LABEL_TOO_LONG    (1 << 4)
#define SYNTAX_ERR_SYNTAX_ERROR           (1 << 5)

Atom* tisp_interpreter_read_str(const char* str);
//const char* tisp_interpreter_get_output_str();
uint8_t tisp_interpreter_get_error();

#endif//_TISP_INTERPRETER_H_
