#include <stdio.h>
#include <string.h>

void pwd()
{
    char *PWD_var = (char *)malloc(2048 * sizeof(char));
    if (getcwd(PWD_var, 2048) != NULL)
        printf("%s\n", PWD_var);
    else 
    {
       perror("getcwd() error");
       exit(1);
    }
    free(PWD_var);
    return;
}