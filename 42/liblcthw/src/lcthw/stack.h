#ifndef lcthw_stack_h
#define lcthw_stack_h

#include <lcthw/list.h>

typedef List Stack;

#define Stack_count(S) List_count(S)
#define Stack_create() List_create()
#define Stack_destroy(S) List_destroy(S)
#define Stack_push(S, I) List_push(S, I)
#define Stack_pop(S) List_pop(S)
#define Stack_peek(S) List_last(S)

#define STACK_FOREACH(S, CURR) ListNode *CURR = NULL; for(CURR=S->first; CURR!=NULL; CURR=CURR->next) 

#endif
