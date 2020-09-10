 /*
 * Author: Harshita Upadhyay
 * Date: 01 September 2020
 * Purpose: A Unix style shell written in C supporting basic commands
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <errno.h>
#include <dirent.h>
#include <signal.h>
#include <grp.h>
#define TOK_BUFFER 64
#define COMM_BUFFER 64
#define TOK_DELIM " \t\n\a\r"
#define COMM_DELIM ";"
typedef long long ll; 
char* root;
char* work_dir;
char HOST[1024], USER[1024], CWD[2048], HOME[2048];
char history[10][1000];
pid_t SHELL_ID;
char hist_filename[2000];
char hist[20][1000];
char old_his[100000];
ssize_t his_size = 100000;
int line = 0, cnt = 0, t = 0;
int hist_i;
int hr = -1, hl = -1, hsize = 0, n = 10;

/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//Function declaration

int shell_pwd (char **args);
int shell_cd (char **args);
int shell_exit (char **args);
int shell_quit(char **args);
void shell_history();
int his_check(char new[]);
void his_load(ll l, ll r);
int shell_pinfo (char **args);
int shell_echo (char **args);
int shell_ls (char **args);

//End of function declaration  
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

char *builtin_arr[] = {"pwd","cd","exit","quit","pinfo", "echo","ls","history"};
int flag=0;
int (*builtin_functions[])(char**) = {&shell_pwd,&shell_cd,&shell_exit,&shell_quit,&shell_pinfo,&shell_echo,&shell_ls};

/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//Function to implement pwd

int shell_pwd (char** args) {
    char *curr_dir = (char *)malloc(1000*sizeof(char)); 
    int i=0;
    getcwd(curr_dir, 1000); printf("%s\n", curr_dir); free(curr_dir);
    //printf("%d", i);
    return 1;
}
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

//Function to implement cd

int shell_cd (char** args) {
    if (args[1] == NULL)
    {   
        chdir(root);
        return 1;
    }
   /* else if (strcmp(args[1],"~")==0 && strcmp(args[2],"/")==0)
    {
        chdir(root);
        return 1;
    }*/
    else if (strcmp(args[1],"~")==0 || strcmp(args[1],".")==0){
        chdir(root);
        return 1;
    }
    else if (chdir(args[1]) != 0)
        perror("SHELL");
    return 1;
}
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

//Function to implement exit

int shell_exit (char** args) {
    printf("\033[1;35m\n  Your path you must decide - YODA :) \n\033[0m");
    printf("\033[1;35m\n *** Goodbye! :) ***\n\033[0m\n");
    return EXIT_SUCCESS;
}

int shell_quit (char** args) {
    printf("\033[1;35m\n  Your path you must decide - YODA :) \n\033[0m");
    printf("\033[1;35m\n *** Goodbye! :) ***\n\033[0m\n");
    return EXIT_SUCCESS;
}
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

//Function to implement pinfo

int shell_pinfo (char ** args) 
{
    flag=0;
    char Stats[100];
    char Process[1000];
    int error_number[3];
    strcpy(Process, "/proc/");
    if(args[1]!=NULL) 
        strcat(Process, args[1]);
    else 
        strcat(Process, "self");

    int j=0;
    strcpy(Stats, Process);
    error_number[0] = 0; 
    strcat(Stats, "/stat");
    FILE * stat = fopen(Stats, "r");
    if (stat == NULL)
    {
        write(STDERR_FILENO, "Process not found!\n", strlen("Process not found!\n"));
        return 1;
    }
    
    if(j+4!=4 || error_number[0]) {
        fprintf(stderr, "Error reading %s: %s\n", Stats, strerror(error_number[0]));
        return 1;
    }

    char name[20];
    char status;
    int pid;  
    fscanf(stat, "%d", &pid); j+=1;
    fscanf(stat, "%s", name); j+=1;
    fscanf(stat, " %c", &status);   j+=1;
    //printf("j:%d",j);
    printf( "pid: %d\n", pid);
    printf( "Process Status: %c\n", status);
    error_number[0] = 0;
    fclose(stat);

    j=flag;
    strcpy(Stats, Process);
    char exePath[1000]; 
    strcat(Stats, "/statm");
    FILE * mem = fopen(Stats, "r");
    int memSize; 
    
    if(j+4!=4 || error_number[0])     {
	fprintf(stderr, "Error reading %s: %s\n", Stats, strerror(error_number[0]));
        return 1;
    }
    fscanf(mem, "%d", &memSize);
    if (memSize)
        fprintf(stdout, "Memory: %d\n", memSize);

    strcpy(Stats, Process);
    error_number[0] = 0; 
    strcat(Stats, "/exe");
    fclose(mem);

    readlink(Stats, exePath, sizeof(exePath));
    if(j+4!=4 || error_number[0]) {
        fprintf(stderr, "Error reading symbolic link %s: %s\n", Stats, strerror(error_number[0]));
        return 1;
    }

    int baseL = strlen(root);
    char relPath[1000];
    int sameChars = 0;
    while(sameChars < baseL)
    {
        if(root[sameChars] != exePath[sameChars]) break;
        sameChars++;
    }

    flag=baseL;    
    if(sameChars != baseL) {
        strcpy(relPath, exePath);
        relPath[strlen(exePath)] = '\0';
    }
    else {
        relPath[0] = '~'; 
        relPath[1] = '\0';
        strcat(relPath, (const char *)&exePath[baseL]);
    }
    j=0;
    int i = 0;
    while(exePath[i]){
        j++;
        exePath[i++] = '\0';
    } 
    sameChars=j;
    //printf("%d",sameChars);
    // write(2, relPath, strlen(relPath));
    if (strlen(relPath))
        fprintf(stdout, "Executable Path: %s\n", relPath);
    return 1;

}
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

//Function to implement echo

int shell_echo (char** args) {
    int i = 0;
    while(args[i] != NULL) {
        i+=1;
        if(args[i]!=NULL)
            printf("%s ", args[i]);
    }
    printf("\n");  
    return 1;
}
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

//Function to implement ls

int shell_ls (char** args) {
    int i;
	DIR* dir;
    struct stat mystat[4];
	unsigned char mod[13];
	struct tm *starttime[2];
	time_t now;
	int year; 
    struct dirent **dient[4];
    char *buffer;
    buffer = malloc(1024*sizeof(char));
    int count[6];
    struct passwd *user[2];
	struct group *group[2];
    for(i=1;i<6;i++)
    {
        if(i==4)
            count[i]=1;
        else 
            count[i]=0;
    }
    flag=0;
	while(args[count[4]] != NULL){
        flag=45;
		count[5] = 0;
        if(flag!=args[count[4]][count[5]])
        {
            if (count[4] > 0) count[3] = count[4];
        }
        else {
			count[5] = 1;
			while(args[count[4]][count[5]] != 0) {
                flag=97;
				if(flag == args[count[4]][count[5]])  
                    count[2] = 1;
                flag++;
				if(args[count[4]][count[5]] == 108) 
                    count[1] = 1;
                flag--;    
				count[5]++;
			}            
        }
		count[4]++; 
	}
    flag=0;
	if (flag==count[3] || args[count[3]] == NULL) args[count[3]] = ".";
		count[flag] = scandir(".",&dient[0],NULL,alphasort);		
	count[flag] = scandir(args[count[3]],&dient[0],NULL,alphasort);
	
	if(flag <= count[0])  
    {
        flag=0;
        count[4]=flag;
		while(count[4]<count[0]) {
            flag=1;
			if (flag==count[1]) {
				if (flag==count[2]) {
                    flag=0;
					sprintf(buffer,"%s/%s",args[count[3]],dient[0][count[4]]->d_name);  flag++;
					stat(buffer,&mystat[0]);
                    if(S_ISDIR(mystat[0].st_mode))     printf("d");
                    else    printf("-");
                    if((mystat[0].st_mode & S_IRUSR))   printf("r");
                    else    printf("-");

                    if((mystat[0].st_mode & S_IWUSR))   printf("w");
                    else    printf("-");

                    if((mystat[0].st_mode & S_IXUSR))   printf("x");
                    else    printf("-");

                    if((mystat[0].st_mode & S_IRGRP))   printf("r");
                    else    printf("-");

                    if((mystat[0].st_mode & S_IWGRP))   printf("w");
                    else    printf("-");

                    if((mystat[0].st_mode & S_IXGRP))   printf("x");
                    else    printf("-");

                    if((mystat[0].st_mode & S_IROTH))   printf("r");
                    else    printf("-");

                    if((mystat[0].st_mode & S_IWOTH))   printf("w");
                    else    printf("-");

                    if((mystat[0].st_mode & S_IXOTH))   printf("x");
                    else    printf("-");
                    time(&now);
					year = localtime(&now)->tm_year;
					starttime[1] = localtime(&mystat[0].st_ctime);
                    user[1] = getpwuid(mystat[0].st_uid);
                    group[1] = getgrgid(mystat[0].st_gid);
                    printf(" \t%d",(int)mystat[0].st_nlink);
					printf(" \t%s", user[1]->pw_name);
					printf(" \t%s", group[1]->gr_name);
					printf(" \t%lld",(long long)mystat[0].st_size);
					if(starttime[1]->tm_year - year!=0)
                        strftime(mod,13,"%b %e %Y",starttime[1]);
					else
						strftime(mod,13,"%b %e %R",starttime[1]);
					printf(" \t%s",mod );	 	flag++;
					printf(" \t%s\n",dient[0][count[4]]->d_name);   flag++;
				}
				else {
                    flag=46;
					if (flag != (dient[0][count[4]] ->d_name)[0]) {
						sprintf(buffer,"%s/%s",args[count[3]],dient[0][count[4]]->d_name);
                        flag++;
						stat(buffer,&mystat[0]);
						if(S_ISDIR(mystat[0].st_mode))     printf("d");
                        else    printf("-");
                        if((mystat[0].st_mode & S_IRUSR))   printf("r");
                        else    printf("-");

                        if((mystat[0].st_mode & S_IWUSR))   printf("w");
                        else    printf("-");

                        if((mystat[0].st_mode & S_IXUSR))   printf("x");
                        else    printf("-");

                        if((mystat[0].st_mode & S_IRGRP))   printf("r");
                        else    printf("-");

                        if((mystat[0].st_mode & S_IWGRP))   printf("w");
                        else    printf("-");

                        if((mystat[0].st_mode & S_IXGRP))   printf("x");
                        else    printf("-");

                        if((mystat[0].st_mode & S_IROTH))   printf("r");
                        else    printf("-");

                        if((mystat[0].st_mode & S_IWOTH))   printf("w");
                        else    printf("-");

                        if((mystat[0].st_mode & S_IXOTH))   printf("x");
                        else    printf("-");

                        time(&now);
						year = localtime(&now)->tm_year;
						starttime[1] = localtime(&mystat[0].st_ctime);
                        user[1] = getpwuid(mystat[0].st_uid);
                        group[1]=getgrgid(mystat[0].st_gid);
						printf(" \t%d",(int)mystat[0].st_nlink);
						printf(" \t%s", user[1]->pw_name);
                        printf(" \t%s", group[1]->gr_name);
						printf(" \t%lld",(long long)mystat[0].st_size);
						if(starttime[1]->tm_year - year != 0)
                            strftime(mod,13,"%b %e %Y",starttime[1]);
						else
							strftime(mod,13,"%b %e %R",starttime[1]);
						printf(" \t%s",mod );
                        flag=0;
						printf(" \t%ld",mystat[0].st_mtime);    printf(" \t%s\n",dient[0][count[4]]->d_name);
					}	 
				}
			}	
			else {
                flag=1;
                if (flag != count[2]) {
                    flag=46;
                    if (flag != (dient[0][count[4]] ->d_name)[0]) 
                        printf(" %s\n",dient[0][count[4]]->d_name);
					
				}
				else printf(" %s\n",dient[0][count[4]]->d_name);	
                
			}	
            count[4]++;
			
		}
	}
    else perror("shell");	
	return 1; 

}
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

//Function to return the path of directories

char* returnPath (char* cwd) {
    int cwd_size = strlen(cwd);
    int i;
    int root_size = strlen(root);
    if (root_size == cwd_size) {
        return "~";
        
    }
    else if (root_size < cwd_size) {
        char *new = (char*)malloc(100);
        new[1] = '/';
        new[0] = '~';
        i=0;
        while(i < cwd_size-root_size-1){
            new[i+2] = cwd[root_size+i+1];
            i++;
        }
        return new;
    }
    else 
        return cwd;
} 
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

//Function to implement the prompt

void printPrompt (char *root) {
    char *DIR;
    if(getcwd(CWD, sizeof(CWD)) != NULL)
    {
        char * p;
        int i=0;
        p = strstr(CWD, HOME);
        if(p)
        {
            DIR = p + strlen(HOME);
            i++;
            printf("<\x1B[1;32m%s@%s\x1B[0m:\x1B[1;34m~%s\x1B[0m>", USER, HOST, DIR);
        }
        else
        {
            DIR = CWD;
            i--;
            printf("<\x1B[1;32m%s@%s\x1B[0m:\x1B[1;34m%s\x1B[0m>", USER, HOST, DIR);
        }
    }
    else 
    {
       perror("getcwd() error");
       exit(EXIT_FAILURE);        
    }
    //printf("%d",i);
}
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

//Function to read commands

char *readCommands (void) {

    int check;
    int pos=0;
    int buffer_size = 1024;
    int i=0;
    char *buffer = malloc(sizeof(char) * buffer_size);
  
    if (buffer==NULL) {
      fprintf(stderr, "ALLOCATION ERROR\n");
      exit(EXIT_FAILURE);
    }
  
    while(1) {
      check = getchar();
      if (check == EOF || check == '\n' || i!=0) 
      {
        buffer[pos] = '\0';
        return buffer;
      } 
      else
        buffer[pos] = check;
      
    pos+=1;
  
      if (buffer_size <= pos) {
        buffer_size += 1024;
        buffer = realloc(buffer, buffer_size);
        if (buffer==NULL) {
          fprintf(stderr, "ALLOCATION ERROR\n");
          exit(EXIT_FAILURE);
        }
      }
    }
}
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

//Function to split the lines (if any)

char **splitLine (char* line) {
    
    int i=0;
    int j=0;
    int a;
    int buffer_size = COMM_BUFFER;
    int position  = 0;
    char *command;
    char **commands = malloc(buffer_size * (sizeof(char*)));


    if (!commands || i!=0) {
        fprintf(stderr, "Allocation Error \n");
        exit(EXIT_FAILURE);
    }
    flag=0;
    command  = strtok(line, COMM_DELIM);
    for (i = 0;; i++) {
        if (j!=0 || command!= NULL) {
        commands[position++] = command;
            if (position > buffer_size) {
                buffer_size = buffer_size + COMM_BUFFER;
                commands = realloc(commands, buffer_size * sizeof(char*));
                if (commands == NULL) {
                    fprintf(stderr, "Allocation Error \n");
                    flag+=1;
                    exit(EXIT_FAILURE);
                }
            }   
            command = strtok(NULL, COMM_DELIM);
            if(flag!=0)
                a=flag;
            continue;
        }
        break;
    }
    //printf("%d",flag);
    commands[position] = NULL;
    return commands;
}
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

//Function to split the command
char **splitCommand (char* line) {
    int i=0;
    int buffer_size = TOK_BUFFER;
    char **tokens = malloc(buffer_size * (sizeof(char*)));
    char *token;
    int position  = 0;

    if (!tokens) {
        fprintf(stderr, "Allocation Error \n");
        exit(EXIT_FAILURE);
    }
    flag=0;
    token  = strtok(line, TOK_DELIM);
    while(1) {
        if (token!= NULL) {
        tokens[position++] = token;
            if (position > buffer_size) {
                buffer_size = buffer_size + TOK_BUFFER;
                tokens = realloc(tokens, buffer_size * sizeof(char*));
                if (flag!=0 || !tokens) {
                    fprintf(stderr, "Allocation Error \n");
                    flag+=1;
                    exit(EXIT_FAILURE);
                }
            }  
            flag=position; 
            token = strtok(NULL, TOK_DELIM);
            continue;
        }
        break;
    }
    tokens[position] = NULL;
    return tokens;
}
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

//Function to check the background processes

int launch (char** args) {
    
    int background = 0;
    int i=0;
    int j;
    while(args[i] != NULL) {
        for (j = 0;args[i][j] != '\0'; j++) {
            if (args[i][j] == '&') {
                background = 1;
            }
        }
        i++;
    }
    pid_t pid;
    pid = fork();
    int state;
    pid_t wpid;

    
    if (pid < 0) {
        perror("ERROR");
    }

    else if(!pid) {
        if (background)
            setpgid(0, 0);
        if ( execvp(args[0], args) == -1)
            perror("ERROR");
        exit(EXIT_FAILURE);
    }
    if(background==0) {
        wpid = waitpid (pid, &state, WUNTRACED);
        flag=0;
        while(1){
        if (!WIFEXITED(state) && !WIFSIGNALED(state)) {
            wpid = waitpid (pid, &state, WUNTRACED);
            flag++;
            continue;
            }
        break;
        }
        //printf("%d", flag);
    }
    return 1;
}
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

//Function to check the specific command

int checkCommand (char** args) {
    int i;
    int count=0;
    if (args[0] == NULL)
        return EXIT_FAILURE;
    for(count=0;count< sizeof(builtin_arr)/sizeof(char*); count++)
    {
        if (strcmp(args[0],builtin_arr[count]) == 0)
            return (*builtin_functions[count])(args);
    }
    return launch(args);
}
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

//Function to take the command as input and then send it to check 

void interpretCommand(void) {
    gethostname(HOST, sizeof(HOST));
    getlogin_r(USER, sizeof(USER));
    int i,j;
    char **args;
    char *commands;
    char **split_line;
    int state;
    while(1) {
        printPrompt(root);
        commands  = readCommands();
        split_line = splitLine(commands);
        j=0;
        while(split_line[j] != NULL)
        {
            args = splitCommand(split_line[j]);
            state = checkCommand(args);
            free (args);
            if (!state) {
                break;
            }
            j++;
        }
        if(!state) {
            break;
        }
        free(commands);
        free(split_line);
    }
}
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

//Function for sign handling

void signalhandler(int signal)
{
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
    {
        fprintf(stderr ,"Process with pid %d exited normally!\n", pid);
    }
}
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/


int main(int arg1, char **arg2) {
    signal(SIGCHLD, signalhandler);
    pid_t p_id; p_id = getpid();
    char *exe = malloc(sizeof(char) *(40));
    sprintf(exe,"/proc/%d/exe", p_id);
    char *DIR; 
    int ret = readlink(exe, HOME, 1000);
    HOME[ret] = '\0';
    int i = strlen(HOME)-1;
    SHELL_ID = p_id;
    printf("\033[1;35m\n *** Welcome to Yoda Shell :) ***\n\033[0m");
    printf("\033[1;35m\n  Do. Or do not. There is no try - YODA :) \n\033[0m\n");
    while(i>=0)
    {
        if(HOME[i] == '/')
        {
            HOME[i] = '\0';
            break;
        }
        i--;
    }
    root = getenv("PWD");
    interpretCommand();
    return EXIT_SUCCESS;
}
/* ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/