/** WARNING: This code is fresh and potentially isn't correct yet. */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "dbg.h"

#define MAX_DATA 100

const char digits[10] = {'0','1','2','3','4','5','6','7','8','9'};

int read_string(char **out_string, int max_buffer)
{
    *out_string = calloc(1, max_buffer + 1);
    check_mem(*out_string);

    char *result = fgets(*out_string, max_buffer, stdin);
    check(result != NULL, "Input error.");

    return 0;

error:
    if(*out_string) free(*out_string);
    *out_string = NULL;
    return -1;
}

int read_int(int *out_int)
{
    char *input = NULL;
    int rc = read_string(&input, MAX_DATA);
    check(rc == 0, "Failed to read number.");

    *out_int = atoi(input);

    free(input);
    return 0;

error:
    if(input) free(input);
    return -1;
}

int read_scan(const char *fmt, ...)
{
    int i = 0;
    int rc = 0;
    int *out_int = NULL;
    char *out_char = NULL;
    char **out_string = NULL;
    int max_buffer = 0;

    va_list argp;
    va_start(argp, fmt);

    for(i = 0; fmt[i] != '\0'; i++) {
        if(fmt[i] == '%') {
            i++;
            switch(fmt[i]) {
                case '\0':
                    sentinel("Invalid format, you ended with %%.");
                    break;

                case 'd':
                    out_int = va_arg(argp, int *);
                    rc = read_int(out_int);
                    check(rc == 0, "Failed to read int.");
                    break;

                case 'c':
                    out_char = va_arg(argp, char *);
                    *out_char = fgetc(stdin);
                    break;

                case 's':
                    max_buffer = va_arg(argp, int);
                    out_string = va_arg(argp, char **);
                    rc = read_string(out_string, max_buffer);
                    check(rc == 0, "Failed to read string.");
                    break;

                default:
                    sentinel("Invalid format.");
            }
        } else {
            fgetc(stdin);
        }

        check(!feof(stdin) && !ferror(stdin), "Input error.");
    }

    va_end(argp);
    return 0;

error:
    va_end(argp);
    return -1;
}

typedef struct char_node {
    struct char_node * next;
    char data;
    int tail_size;
} node;

node* digit_to_char_recursion(int number, node* tail)
{
    int digit = number % 10;
    int next_number = (number - digit) / 10;
    
    node* next_node = malloc(sizeof(node));
    next_node->data = digits[digit];
    
    if (tail == NULL)
    {
        next_node->tail_size = 1;              
    }
    else 
    {
        next_node->tail_size = tail->tail_size + 1;
        next_node->next = tail;
    }
    
    if (next_number == 0) 
    { 
        return next_node;
    }
    else 
    {
        return digit_to_char_recursion(next_number, next_node);
    }
}

char* itoa(int number)
{
    node* head = digit_to_char_recursion(number, NULL);
    char* str = malloc(sizeof(char) * ((head->tail_size) + 1));
    int i = 0;
    while(head != NULL)
    { 
        str[i++] = head->data;
        node* tmp_head = head->next;
        free(head);
        head = tmp_head;
    }
    str[i] = '\0';
    return str; 
}

int print_powerman(const char *fmt, ...)
{
    va_list argp;
    va_start(argp, fmt);
    int count = 0;
    int out_int;
    char out_char;
    char* out_string;
    for(int i = 0; fmt[i] != '\0'; i++)
    {
        if (fmt[i] == '%')
        {
            i++;
            switch(fmt[i]) 
            {
                case '\0':
                    sentinel("Invalid format, you ended with %%.");
                    break;
                case 'd':
                    out_int = va_arg(argp, int);
                    char *num_str = itoa(out_int);
                    fputs(num_str, stdout);
                    count += strlen(num_str);
                    free(num_str);     
                    break;
                case 'c':
                    out_char = va_arg(argp, char);
                    fputc(out_char, stdout);
                    ++count;
                    break;
                case 's':
                    out_string = va_arg(argp, char *);
                    fputs(out_string, stdout);
                    count += strlen(out_string);
                    break;
                default:
                    sentinel("Invalid format.");
            }
        }
        else
        { 
            fputc(fmt[i], stdout);
            ++count;            
        }
    }  
    va_end(argp);
    return count;
error:
    va_end(argp);
    return -1;
}

int main(int argc, char *argv[])
{
    char *first_name = NULL;
    char initial = ' ';
    char *last_name = NULL;
    int age = 0;

    printf("What's your first name? ");
    int rc = read_scan("%s", MAX_DATA, &first_name);
    check(rc == 0, "Failed first name.");

    print_powerman("What's your initial? ");
    rc = read_scan("%c\n", &initial);
    check(rc == 0, "Failed initial.");

    printf("What's your last name? ");
    rc = read_scan("%s", MAX_DATA, &last_name);
    check(rc == 0, "Failed last name.");

    printf("How old are you? ");
    rc = read_scan("%d", &age);

    printf("---- RESULTS ----\n");
    print_powerman("First Name: %s", first_name);
    print_powerman("Initial: '%c'\n", initial);
    print_powerman("Last Name: %s", last_name);
    print_powerman("Age: %d\n", age);

    free(first_name);
    free(last_name);
    return 0;
error:
    return -1;
}
