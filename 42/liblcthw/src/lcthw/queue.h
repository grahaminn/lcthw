#ifndef lcthw_queue_h
#define lcthw_queue_h

#include <lcthw/list.h>

typedef List Queue;

#define Queue_count(Q) List_count(Q)
#define Queue_create() List_create()	
#define Queue_destroy(Q) List_destroy(Q)
#define Queue_send(Q, I) List_push(Q, I) 
#define Queue_recv(Q) List_remove(Q, Q->first)
#define Queue_peek(Q) List_first(Q)

#define QUEUE_FOREACH(Q, CURR) ListNode *CURR = NULL; for(CURR=Q->first; CURR!=NULL; CURR=CURR->next)

#endif
