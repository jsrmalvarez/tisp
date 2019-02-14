#ifndef _TISP_CONFIG_H
#define _TISP_CONFIG_H

#ifdef DEBUG
  #define SPAM(a) printf a
#else
  #define SPAM(a) (void)0
#endif
	
#define MAX_ATOM_STR_SIZE 16
#define MAX_FUN_PARAMS 5
#define MAX_FUN_RECURSION 2//4
#define MAX_TREE_ELEMENTS 25 //256  //MAX_FUN_PARAMS^MAX_FUN_RECURSION
#define MAX_FUN_LABEL_STR_SIZE MAX_ATOM_STR_SIZE


#endif//_TISP_CONFIG_H_



