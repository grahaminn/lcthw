#ifndef lcthw_List_algos_h
#define lcthw_List_algos_h

#include "list.h"

typedef int (*List_compare)(void* source_data, void* destination_data);

int List_bubble_sort(List *list, List_compare comparator);

List *List_merge_sort(List *list, List_compare comparator);

#endif
