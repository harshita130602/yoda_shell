#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
extern char s[10000]; 
extern char s_save[10000];
int stx;
void strrem(char *s, char c){ 
	int j=0;
    int n = strlen(s); 
	for (int i=0; i<n; ++i)
    {
        if (c != s[i]) s[j++] = s[i];

		s[j] = '\0'; 
    } 
}

void cmd_echo()
{
	char* tok;
    int k=0;
	tok = strtok(s," ");
	if(strcmp(tok,"echo")==0)
	{
		tok = strtok(0,"");
		strrem(tok,'"');
		strrem(tok,'\'');
        stx=1;
		printf("%s\n", tok);
	}
	strcpy(s,s_save);
}