#include "minunit.h"
#include <lcthw/ringbuffer.h>
#include <assert.h>

RingBuffer *buffer = NULL;

char *test_create()
{
	buffer = RingBuffer_create(5);
	mu_assert(buffer, "The buffer is null");
	mu_assert(buffer->length == 6, "The buffer does not have the expected 6 elements");
	return NULL;	
}

char *test_read_write()
{
	RingBuffer_write(buffer, "abc", 3);
	RingBuffer_write(buffer, "def", 3);
	char *tmp = calloc(sizeof(char), 3);
	RingBuffer_read(buffer, tmp, 3);
	if (tmp[0] != 'f')
	{
		fprintf("buffer contains %s", tmp);
	}
	mu_assert(tmp[0] == 'f', "Don't get it");
	return NULL;
}

char *test_destroy()
{
	RingBuffer_destroy(buffer);
	return NULL;
}

char *all_tests() 
{
	mu_suite_start();
	mu_run_test(test_create);	
	mu_run_test(test_read_write);
	mu_run_test(test_destroy);

	return NULL;
}

RUN_TESTS(all_tests);
