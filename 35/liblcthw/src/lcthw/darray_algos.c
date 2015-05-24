#include <lcthw/darray_algos.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int DArray_qsort(DArray *array, DArray_compare cmp)
{
    qsort(array->contents, DArray_count(array), sizeof(void *), cmp);
    return 0;
}

void swap(void *data1, void *data2, size_t size)
{
	void *tmp = malloc(size);
	memcpy(tmp, data2, size);
	memcpy(data2, data1, size);
	memcpy(data1, tmp, size);
	free(tmp);
}

void siftDown(void *data, int start, int end, size_t size, DArray_compare cmp)
{
	int root = start;
	while ((root * 2 + 1) <= end)
	{
		int child = root * 2 + 1;
		int swap_index = root;
         
		if (cmp(data+swap_index*size, data+child*size) < 0)
		{
			swap_index = child;
		}


		if (((child + 1) <= end) && (cmp(data+swap_index*size, data+(child + 1)*size) < 0))
		{
			swap_index = child + 1;
		}

		if (swap_index != root)
		{
			swap(data+root*size, data+swap_index*size, size);
			root = swap_index;
		}
		else return;
	}
}

void heapify(void *data, int count, size_t data_size, DArray_compare cmp)
{
	int start = floor((count - 2) / 2);
	while (start >= 0)
	{
		siftDown(data, start, count - 1, data_size, cmp);
		--start;
	}
}

int my_heapsort(void *data, int count, size_t data_size, DArray_compare cmp)
{
	heapify(data, count, data_size, cmp);
	int end = count - 1;  
	while (end > 0) 
	{
		swap(data+end*data_size, data, data_size);
		--end;
		siftDown(data, 0, end, data_size, cmp);
	}
	return 0;
}


int DArray_heapsort(DArray *array, DArray_compare cmp)
{
    return my_heapsort(array->contents, DArray_count(array), sizeof(void *), cmp);
}

int DArray_mergesort(DArray *array, DArray_compare cmp)
{
    return mergesort(array->contents, DArray_count(array), sizeof(void *), cmp);
}
