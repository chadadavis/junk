/* An example of two-level string tokenization with the re-entrant strtok_r */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char** argv) {

    if (argc < 3) {
        fprintf(stderr, 
                "Usage: %s \"field:another field:last field\" \": \"\n",
                argv[0]);
        exit(1);
    }
    char* str = argv[1];
    char* delims = argv[2];

    /* Save ptrs for re-entrant version of strtok_r */
    char* ptr1;
    char* ptr2;
    /* Delimiter characters */
    char delim1[] = {delims[0],'\0'};
    char delim2[] = {delims[1],'\0'};

    char* res1 = strtok_r(str, delim1, &ptr1);
    while (res1) {
        printf(":%s:\n", res1);
        char* res2 = strtok_r(res1, delim2, &ptr2);
        while (res2) {
            printf("\t:%s:\n", res2);
            /* NULL states to keep parsing the same string, 
               ptr2 identifies which string is "this" string
               I.e. ptr2 is just an arbitrary identifying token
            */
            res2 = strtok_r(NULL, delim2, &ptr2);
        }
        res1 = strtok_r(NULL, delim1, &ptr1);
    }
}

    
