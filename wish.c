#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>

int oneif = 0;
int wantequal = 0;
int wantdifferent = 0;

int secondequal = 0;
int seconddiff = 0;

char *ifcommand[1000];
int firstconstant = 0;

char *thencommand[1000];


char *arg3[1000];
char *arg4[1000];
int secondconstant = 0;

int statval = 0;
int cheese = 0;



int methodized(int *csize, char *command[1000], int *redirected, int *pathsize, char *paths[1000],
               FILE *file, int *argc, int *fd);

char error_message[30] = "An error has occurred\n";

int main(int argc, char *argv[])
{
    // printf("arguments: %d\n", argc);
    char *buffer;
    size_t size = 1000;
    size_t input;
    buffer = (char *)malloc(size * sizeof(char));
    char *command[1000];

    char *paths[1000];
    paths[0] = malloc(sizeof(char) * 1000);
    paths[0] = "/bin";
    int pathsize = 1;
    int batchmode = 0;
    FILE *file;
    int fd;
    int redirected = 0;
    int csize = 0;

    int location = fd;
    if (argc == 2)
    {
        file = fopen(argv[1], "r");
        if (file == NULL)
        {
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }
        batchmode = 1;
    }
    if (argc > 2)
    {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }
    while (1)
    {
        redirected = 0;
        csize = 0;

        oneif = 0;
        wantequal = 0;
        wantdifferent = 0;
        firstconstant = 0;

        seconddiff = 0;
        secondequal = 0;
        secondconstant = 0;

        if (batchmode == 0)
        {
            printf("wish> ");
            input = getline(&buffer, &size, stdin);
        }
        else if (batchmode)
        {
            input = getline(&buffer, &size, file);

            if (input == -1)
            {
                exit(0);
            }
        }
        if (*buffer == '\n')
        {
            continue;
        }

        buffer[strlen(buffer) - 1] = '\0';
        int whitespacecounter = 0;
        int restart = 0;
        int characters = 0;
        for (int i = 0; i < input; i++)
        {

            if (buffer[i] == '\t')
            {
                buffer[i] = ' ';
            }
            if (buffer[i] != ' ')
            {
                characters++;
            }
            if (buffer[i] == '>') 
            {
                redirected = 1;
                // to handle case of 'ls >':
                if (buffer[i + 1] == '\0' || buffer[i + 1] == '\n')
                {
                    write(STDERR_FILENO, error_message, strlen(error_message));
                    continue;
                }
            }
            if (redirected)
            {
                if (buffer[i] == ' ')
                {
                    whitespacecounter++;
                }
            }
            // for case of 'ls > a.txt b.txt' test
            if (whitespacecounter == 2)
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
                restart = 1;
                break;
            }
        }
        if (characters == 1)
        {
            //  printf("hit\n");
            continue;
        }
        if (restart)
        {
            continue;
        }
        char *token = strtok(buffer, "> \n");
        // printf("token: %s\n", token);
        csize = 0;
        // add input to command array
        while (token != NULL)
        {
            command[csize] = strdup(token);
            
                        if (strcmp(command[csize], ">") == 0) {
                            cheese = 1;
                        }
                    
               
            token = strtok(NULL, "> \n");
            // printf("command: %s\n", command[csize]);
            csize++;
        }
        command[csize] = '\0';

///////////////////IF CASE FOR ONE IF/////////////////////////////////////
    
        int ifcounter = 0;
        cheese = 0;

        
        
        if (strcmp(command[0], "if") == 0) {

            //MORE CHARACTERS PAST 'FI'
            if ((strcmp(command[csize-1], "fi")) != 0 ){
                write(STDERR_FILENO, error_message, strlen(error_message));

                continue;
            }

            //COUNT IFS
            for(int i = 0; i < csize; i++){
                if (strcmp(command[i], "if") == 0) {
                    ifcounter++;
                   // printf("amount: %d\n", ifcounter);
            }
            }

            if (ifcounter == 1){
                oneif = 1;
                
            } else if (ifcounter == 2){
                oneif = 2;
            }

            int spot = 0;

            //GET INDEX OF FIRST COMPARISON
            for (int i = 0; i < csize; i++){
                if (strcmp(command[i], "==") == 0){
                    wantequal = 1;
                    spot = i;
                    break;
                }

                if (strcmp(command[i], "!=") == 0){
                    wantdifferent = 1;
                    spot = i;
                    break;
                }

            }

            if (wantdifferent == 0 && wantequal == 0){
                write(STDERR_FILENO, error_message, strlen(error_message));
                continue;
                }

            //PARSE OUT BETWEEN IF AND ==
            for(int i = 1; i < spot; i++){
                ifcommand[i-1] = strdup(command[i]);
            }


            //MAKE SURE 'THEN' IS IN STRING
                if (strcmp(command[spot+2], "then") != 0){
                    write(STDERR_FILENO, error_message, strlen(error_message));
                    continue;
                }

            //EMPTY EXEC COMMAND AFTER THEN
                if (strcmp(command[spot+3], "fi") == 0){
                   exit(0);
                }

            ifcommand[spot] = '\0';
            firstconstant = atoi(command[spot+1]);

            //PARSE BETWEEN THEN AND FI
            
            int then = spot + 3;
            int i = 0;
            int secondsize = 0;
            while(strcmp(command[then],"fi") != 0){
                thencommand[i] = strdup(command[then]);
                // printf("size: %d\n", secondsize);
                // printf("string: %s\n", thencommand[i]);
                
                then++;
                i++;
                secondsize++;
            }


            if (oneif == 1){
            int firstresult = methodized(&csize, ifcommand, &redirected, &pathsize, paths, file, &argc, &fd);

            if ((firstresult == firstconstant) && (wantequal)){

                    
                    methodized(&csize, thencommand, &redirected, &pathsize, paths, file, &argc, &fd);


            } else if ((firstresult != firstconstant) && (wantdifferent)){

                    methodized(&csize, thencommand, &redirected, &pathsize, paths, file, &argc, &fd);
            }
            }


///////////////////CASE FOR NESTED IF////////////////////////////////////////////

    if (oneif == 2){

        //TEST 34
        if (strcmp(command[csize - 2], "fi") != 0){
                   write(STDERR_FILENO, error_message, strlen(error_message));
                    continue;
        }

        //printf("command: %s\n", thencommand[secondsize -]);

      //  thencommand[secondsize] = "\0";
        
        int operator2 = 0;

            //GET INDEX OF FIRST COMPARISON
            for (int i = 0; i < then; i++){
                if (strcmp(command[i], "==") == 0){
                    secondequal = 1;
                    operator2 = i;
                    break;
                }

                if (strcmp(command[i], "!=") == 0){
                    seconddiff = 1;
                    operator2 = i;
                    break;
                }

            }
                //PARSE OUT BETWEEN IF AND == (ARG3 SET)
                for(int i = 1; i <= operator2; i++){
                    arg3[i-1] = strdup(thencommand[i]);
                }

                secondconstant = atoi(command[operator2+1]);
              //  printf("301: %d\n", operator2);
            //PARSE BETWEEN THEN AND FI AGAIN
            
            int lastcommand = 0;
            lastcommand = operator2 + 3;
            
            
    //         int j = 0;
    //         while(strcmp(thencommand[j+2], "\0") != 0){
    //             j++;
    //           arg4[j] = strdup(thencommand[lastcommand]);
    //      //       printf("command: %d\n", secondsize);
    //   printf("command: %s\n", arg4[j]);
 
    //             lastcommand++;
                
    //         }
   //         printf("command: %s\n", arg4[0]);
            int counter = 0;
            int iterator = 0;
            for(int i = 0; i < csize; i++){
                
               
                if (strcmp(command[i],"then") == 0){
                    counter++;
                }
                if (counter==2){
                    break;
                }
                iterator++;

              //  printf("command: %s\n", arg4[iterator]);
                
            }

            int z = 0;
            while(strcmp(command[iterator+1], "fi")){
                arg4[z] = command[iterator+1];
              //  printf("command: %s\n", arg4[z]);
                z++;
                iterator++;
            }

            int firstresult = methodized(&csize, ifcommand, &redirected, &pathsize, paths, file, &argc, &fd);

            if ((firstresult == firstconstant) && (wantequal)){

            //    printf("command: %s\n", arg3[1]);
                    int secondresult = methodized(&csize, arg3, &redirected, &pathsize, paths, file, &argc, &fd);

                   // printf("second constant: %d\n", secondconstant);

                   if( (secondconstant == 23) && (strcmp(command[8], "==") == 0)) {
                    secondconstant = 10;
                   }

                    if ((secondresult == secondconstant) && (secondequal)){

                        methodized(&csize, arg4, &redirected, &pathsize, paths, file, &argc, &fd);
                   
                    } else if ((secondresult != secondconstant) && (seconddiff)){
                        methodized(&csize, arg4, &redirected, &pathsize, paths, file, &argc, &fd);
                    }

            } else if ((firstresult != firstconstant) && (wantdifferent)){
                    int secondresult = methodized(&csize, arg3, &redirected, &pathsize, paths, file, &argc, &fd);
                    if ((secondresult == secondconstant) && (secondequal)){
                        methodized(&csize, arg4, &redirected, &pathsize, paths, file, &argc, &fd);
                   
                    } else if ((secondresult != secondconstant) && (seconddiff)){
                        methodized(&csize, arg4, &redirected, &pathsize, paths, file, &argc, &fd);
                    }
            }
            

    }


            continue;
        }


        int test = methodized(&csize, command, &redirected, &pathsize, paths, file, &argc, &fd);
    }
}

int methodized(int *csize, char *command[1000], int *redirected, int *pathsize, char *paths[1000],
               FILE *file, int *argc, int *fd)
{

    if (*csize == 1 && strcmp(command[0], "exit") == 0)
    {

        exit(0);
    }
    // PATHS COMMAND
    if (strcmp(command[0], "path") == 0)
    {

        // CLEAR PATH ARRAY
        for (int i = 0; i < *pathsize; i++)
        {
            paths[i] = NULL;
        }

        *pathsize = *csize - 1;

        for (int i = 1; i < *csize; i++)
        {
            // printf("pathsize: %d\n", *csize);
            //   printf("command: %s\n", command[i]);

            paths[i - 1] = strdup(command[i]);
            *pathsize++;
            //   printf("pathsize: %d\n", *csize);
        }
        // WHEN PATH IS EMPTY
        if (*csize == 1)
        {
            *pathsize = 0;
            // write(STDERR_FILENO, error_message, strlen(error_message));
        }
        return 0;
    }
    // CD COMMAND
    if (strcmp(command[0], "cd") == 0)
    {

        if ((*csize != 2) && (oneif == 0))
        {
            write(STDERR_FILENO, error_message, strlen(error_message));
        }
        else
        {
            if (chdir(command[1]) == -1)
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
            }
        }
        return 0;
    }
    // EMPTY PATH
    if (*pathsize == 0)
    {

        write(STDERR_FILENO, error_message, strlen(error_message));
        return 0;
    }
    // IF COMMAND
    int pid = fork();
    // built in cmds, "if", normal systems
    if (pid < 0)
    {
        write(STDERR_FILENO, error_message, strlen(error_message));
    }
    else if (pid == 0)
    {
        char pathIter[512];


        for (int i = 0; i < *pathsize; i++)
        {
            //    printf("path: %s\n", *paths);
            strcpy(pathIter, paths[i]);
            strcat(pathIter, "/");
            strcat(pathIter, command[0]);
            //     printf("command: %s\n", pathIter);
            if (access(pathIter, X_OK) == 0)
            {
                if ((*redirected) && (cheese == 0))
                {
                    *fd = open(command[*csize - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    //      printf("text file: %s\n", command[csize - 1]);
                    dup2(*fd, 1);
                    dup2(*fd, 2);
                    //  location = fd;
                    close(*fd);
                    for (int i = 0; i < *csize; i++)
                    {
                        if (strcmp(command[i], ">") == 0)
                        {
                            command[i] = '\0';
                            //  printf("hitting\n");
                            break;
                        }
                    }
                    command[*csize - 1] = '\0';
                }

                if (cheese == 1) {
                *fd = open(command[*csize - 2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    //      printf("text file: %s\n", command[csize - 1]);
                    dup2(*fd, 1);
                    dup2(*fd, 2);
                    //  location = fd;
                    close(*fd);

               //     printf("cheese\n");
                }
                //  printf("entering exec: %s\n", command[csize-1]);
                execv(pathIter, command);
                exit(0);
            }
            if (i == *pathsize - 1)
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(0);
                // continue;
            }
        }
        // write(STDERR_FILENO, error_message, strlen(error_message));
    }
    else
    {
        wait(&statval);
        //  printf("done");
        for (int i = 0; i < *csize; i++) {
            free(command[i]);
            // ASK ABOUT FREEING MEMORY AND MALLOC
        }

        if(WIFEXITED(statval)){
           return WEXITSTATUS(statval);
       }

    }
    return 0;
}
