/** ROCK PAPER SCISSORS **/
/*  BY: DAVID JUSINO	**/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    // Check command line arguements.
    if (argc != 2)
    {
        printf("This application requires one positive integer value as a command line arguement\nQuitting..\n");
        return 1;
    } else if (atoi(argv[1]) <= 0){
        printf("This application requires one positive integer value as a command line arguement\nQuitting..\n");
        return 1;
    }

    printf("Rock, Paper, Scissors: Version 1.0\n");
    printf("By David Jusino\n");
    // Create the fork.
    int i;
    i = fork();

    if(i > 0)
    {
        // Set up the server in the parent.
        execl("./referee", argv[1], (char*) NULL);
    }
    else
    {
        // Sleep this child process so the server can properly setup in time.
        sleep(2);

        // Fork off the two Player processes.
        int j;
        j = fork();
        if(j > 0)
        {
            execl("./player", "", (char*) NULL);
        }
        else
        {
            execl("./player", "", (char*) NULL);
        }
    }

    return 0;
}

