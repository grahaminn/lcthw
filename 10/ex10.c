#include <stdio.h>

int main(int argc, char *argv[])
{
    int i = 0;
    int j = 0;
    // go through each string in argv
    // why am I skipping argv[0]?
    for(i = 1, j = 1; i < argc; i++, j++) {
        printf("arg %d: %s\n", i, argv[i]);
    }

    // let's make our own array of strings
    char *states[] = {
        "California", "Oregon",
        "Washington", "Texas",
        argv[1], NULL
    };
    int num_states = 6;

    for(i = 0; i < num_states; i++) {
        printf("state %d: %s\n", i, states[i]);
    }

    return 0;
}
