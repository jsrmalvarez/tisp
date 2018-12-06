#ifndef _TISP_H_
#define _TISP_H_

#include "tisp_interpreter.h"


typedef enum{
  RUNTIME_ERR_NO_ERR,
  RUNTIME_ERR_UNKNOWN_FUN,
  RUNTIME_ERR_NOT_IMPLEMENTED,
  RUNTIME_ERR_BAD_ARITY,
  RUNTIME_ERR_BAD_PARAMETER_TYPE
}RuntimeErr;

RuntimeErr tisp_get_error();

Atom* tisp_eval(Atom* atom);

#endif//_TISP_H_

