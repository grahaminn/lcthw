#include "minunit.h"
#include <lcthw/ringbuffer.h>
#include <assert.h>

RingBuffer *buffer = NULL;

char *test_create()
{
	buffer = RingBuffer_create(7);
	mu_assert(buffer, "The buffer is null");
	mu_assert(buffer->length == 8, "The buffer does not have the expected 8 elements");
	return NULL;	
}

char *test_read_write()
{
    char *foo = "abcdef";
	RingBuffer_write(buffer, foo, 5);

	char *tmp = calloc(sizeof(char), 4);
	RingBuffer_read(buffer, tmp, 4);
	char *tmp2 = calloc(sizeof(char), 1);
	RingBuffer_read(buffer, tmp2, 1);
    printf("buffer contains %s\n", tmp2);	

	char *foo2 = "ghijk";
	char *tmp3 = calloc(sizeof(char), 3);
	RingBuffer_write(buffer, foo2, 5);
	RingBuffer_read(buffer, tmp3, 3);

	printf("buffer contains %s\n", tmp3);
	mu_assert(tmp3[2] == 'i', "Don't get it");
	return NULL;
}

char *all_tests() 
{
	mu_suite_start();
	mu_run_test(test_create);	
	mu_run_test(test_read_write);
	free(buffer);

	return NULL;
}

RUN_TESTS(all_tests);
