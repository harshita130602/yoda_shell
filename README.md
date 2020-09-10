# YODA SHELL - A Linux shell in C
 
## **BRIEF INTRODUCTION**
This shell is a complete working linux shell. It supports most of the linux shell functionalities including semicolon separated commands, all basic linux commands etc. Foreground and Background processes can also be executed. 
## RUN THE CODE
1. Clone this directory and cd into it.
2. Run the command "make".
3. Run ./shell to get a prompt of the form username@system_name:path.
4. Run any command in the shell. It can entail as many number of tabs and spaces, the shell accounts for those.
5. In order to exit, you can run either exit or quit.



## BUILT-IN COMMANDS
The commands that are supported are as follows : 
* pwd -  Prints the present working directory.
* cd  -  Changes directory to the directory specified based on the flags given below:
    ```
        cd
        cd ..
        cd .
        cd ~
        cd <directory>
    ```
    It throws an error if the directory does not exist. 
* echo [arguments] - Displays whatever is specified in [arguments], accounts for double quotes too.
    ```
        echo ["Hello World"]
    ```
* exit OR quit- Exits the shell with return status as success, and adds updates history.txt. 
* ls - Lists the contents of a particular directory based on the flags given below:
    ```
        ls 
        ls -l
        ls -a
        ls -al
        ls -la
        ls -l -a
        ls -a -l
        ls [directory]
    ```
    It throws an error on being executed on something other than a directory.
* pinfo - Lists the details of a particular process (may be based on a process ID that is given). The flags that can be used are given below:
    ```
        pinfo
        pinfo <pid>
    ```
* history - Lists the last 10 commands. 

**Note** - The "Pseudo Home" directory for the shell will be the directory in which the executable resides. Also, all commands other than specified built-in commands are considered as system commands.
## CODE OVERVIEW
* **printPrompt()** - Function to print shell prompt with username and system name along with directory.
* **interpretCommand()** - Function to take the input command and redirect it to the functions like readCommands(), checkCommand() and splitCommands().
* **readCommands()** -  This function basically reads the command as clear by the name.
* **checkCommand()** - This function checks the command from the built-in array (specified in the beginning of the code).
* **splitCommand()** - As clear by the name, this function splits commands(if more than one).
* **launch()** - Function to handle background things.
* **returnPath()** - returns the path of the directory.
* **shell_pwd()** - Executes pwd command and prints the present working directory.
* **shell_pinfo()** - Function to provide details about currently active/running processes. A particular process ID can also be provided as an argument to obtain its details.
* **shell_cd()** - Executes cd command along with all the flags.
* **shell_echo()** - Function to execute echo command
* **shell_ls()** - Executes the ls command along with all the flags.
* **shell_exit()** OR **shell_quit()** - Functions to exit the shell. 
## LICENSE 
This project is licensed under the MIT License - see the [LICENSE.md](https://github.com/harshita130602/yoda_shell/blob/master/LICENSE.md/LICENSE.md) file for details.
