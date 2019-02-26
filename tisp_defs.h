#ifndef _TISP_DEFS_H_
#define _TISP_DEFS_H_
	
#ifdef DEBUG
  #define SPAM(a) printf a
#else
  #define SPAM(a) (void)0
#endif

#include "tisp_config.h"

#endif//_TISP_DEFS_H_
