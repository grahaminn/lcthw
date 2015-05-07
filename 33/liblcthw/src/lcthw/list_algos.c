#include "list_algos.h"
#include <lcthw/dbg.h>

int List_bubble_sort(List* list, List_compare cmp)
{
	if (list->count <= 1) 
	{
		return 0;
	}
	
	while(1)
	{
		int swapped = 0;
		LIST_FOREACH(list, first, next, cur) 
		{
			if (cur->next != NULL)
			{
				if (cmp(cur->value, cur->next->value) > 0) 
				{
					void* temp = cur->value;
					cur->value = cur->next->value;
					cur->next->value = temp;	
					swapped = 1;
				}
			}
		}
        if (swapped == 0) break;
	}
	return 0;  
}

List* sublist(List *list, int start, int end)
{
	check_mem(list);
	List *newList = List_create();
	int pos = 0;
	LIST_FOREACH(list, first, next, cur) 
	{
		if (pos == end)	
		{
			break;
		}
		if (pos++ >= start)
		{
			List_push(newList, cur->value);
		}
	}
	return newList;
error:
	printf("sublist of NULL list");
	return NULL;
}

List* List_merge(List *alist, List *blist, List *destination, List_compare cmp)
{
    ListNode *anode = alist->first;
	ListNode *bnode = blist->first;
	
	if (anode == NULL && bnode == NULL)
	{
		return destination;
	}
	else if (anode == NULL || cmp(anode->value, bnode->value) > 0)
	{
		List_push(destination, bnode);
		List_remove(blist, bnode);
	}
    else if (bnode == NULL)
	{
		List_push(destination, anode->value);
		List_remove(alist, anode);
	}
	else  
	{
		List_push(destination, bnode->value);
		List_remove(blist, bnode);
	}

	return List_merge(alist, blist, destination, cmp);	
}

List* List_merge_sort(List* list, List_compare cmp)
{
	check_mem(list);
	if (list->count <= 1) 
	{
		return list;
	}

	List* listA = sublist(list, 0, list->count / 2);
    List* listB = sublist(list, list->count / 2, list->count); 
	List* result = NULL;
	if (listA == NULL || listA->count==0) return List_merge_sort(listB, cmp);
	else if (listB == NULL || listB->count==0) return List_merge_sort(listA, cmp);
	else 
	{ 
        List* resultList = List_create();
		result = List_merge(List_merge_sort(listA, cmp), List_merge_sort(listB, cmp), resultList, cmp);
	}
	return result;
error:
	printf("List merge on NULL list!");
	return NULL;
}
