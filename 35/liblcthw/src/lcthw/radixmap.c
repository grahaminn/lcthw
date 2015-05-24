/*
* Based on code by Andre Reinald then heavily modified by Zed A. Shaw.
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <lcthw/radixmap.h>
#include <lcthw/dbg.h>

RadixMap *RadixMap_create(size_t max)
{
    RadixMap *map = calloc(sizeof(RadixMap), 1);
    check_mem(map);

    map->contents = calloc(sizeof(RMElement), max + 1);
    check_mem(map->contents);

    map->temp = calloc(sizeof(RMElement), max + 1);
    check_mem(map->temp);

    map->max = max;
    map->max_value = 0;
    map->end = 0;

    return map;
error:
    return NULL;
}

void RadixMap_destroy(RadixMap *map)
{
    if(map) {
        free(map->contents);
        free(map->temp);
        free(map);
    }
}


#define ByteOf(x,y) (((uint8_t *)x)[(y)])

static inline void radix_sort(short offset, uint64_t max, uint64_t *source, uint64_t *dest)
{
    uint64_t count[256] = {0};
    uint64_t *cp = NULL;
    uint64_t *sp = NULL;
    uint64_t *end = NULL;
    uint64_t s = 0;
    uint64_t c = 0;

    // count occurences of every byte value
    for (sp = source, end = source + max; sp < end; sp++) {
        count[ByteOf(sp, offset)]++;
    }

    // transform count into index by summing elements and storing into same array
    for (s = 0, cp = count, end = count + 256; cp < end; cp++) {
        c = *cp;
        *cp = s;
        s += c;
    }

    // fill dest with the right values in the right place
    for (sp = source, end = source + max; sp < end; sp++) {
        cp = count + ByteOf(sp, offset);
        dest[*cp] = *sp;
        ++(*cp);
    }
}

void RadixMap_sort(RadixMap *map, int start_offset)
{
    uint64_t *source = &map->contents[start_offset].raw;
    uint64_t *temp = &map->temp[start_offset].raw;
	int max_radix = (int)ceil(log10(map->max_value));	

	for (int i = 0; i < max_radix; i++)
	{
		radix_sort(i, map->end - start_offset, (i % 2 == 0) ? temp : source, (i % 2 == 0) ? source : temp);
	}
}

RMElement *RadixMap_find(RadixMap *map, uint32_t to_find)
{
    int low = 0;
    int high = map->end - 1;
    RMElement *data = map->contents;

    while (low <= high) {
        uint32_t middle = low + (high - low)/2;
        uint32_t key = data[middle].data.key;

        if (to_find < key) {
            high = middle - 1;
        } else if (to_find > key) {
            low = middle + 1;
        } else {
            return &data[middle];
        }
    }

    return NULL;
}

int RadixMap_find_min_index(int low, int high, RMElement *data, uint32_t to_find)
{
	int mid_index = (high + low) / 2;
	uint32_t mid_value = data[mid_index].data.value;
	if (data[high].data.value <= to_find) 
	{
		return high;
	}
	else if (data[low].data.value > to_find) 
	{
		return low;
	}
	else if (to_find > mid_value)
	{
		return RadixMap_find_min_index(mid_index, high, data, to_find);
	}
	else 
	{
		return RadixMap_find_min_index(low, mid_index, data, to_find);
	}	
}

int RadixMap_add(RadixMap *map, uint32_t key, uint32_t value)
{
    check(key < UINT32_MAX, "Key can't be equal to UINT32_MAX.");

    RMElement element = {.data = {.key = key, .value = value}};
    check(map->end + 1 < map->max, "RadixMap is full.");

    map->contents[map->end++] = element;
	if (map->max_value < value) 
	{	
		map->max_value = value;
	}

	int min_index = RadixMap_find_min_index(0, map->end, map->contents, value);
    RadixMap_sort(map, min_index);

    return 0;

error:
    return -1;
}

int RadixMap_delete(RadixMap *map, RMElement *el)
{
    check(map->end > 0, "There is nothing to delete.");
    check(el != NULL, "Can't delete a NULL element.");

    el->data.key = UINT32_MAX;

    if(map->end > 1) {
        // don't bother resorting a map of 1 length
        RadixMap_sort(map, 0);
    }

    map->end--;

    return 0;
error:
    return -1;
}
