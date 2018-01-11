/** ROCK PAPER SCISSORS **/
/*  BY: DAVID JUSINO	**/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "readstr.h"
#define PLAYER_PATH   "/tmp/player_server"
#define BUFFER_LENGTH      250
#define FALSE              0

void main()
{
    // Variables.
    int STOP = 0;
    int  sd=-1, rc, bytesReceived;
    char buffer[BUFFER_LENGTH];
    struct sockaddr_un serveraddr;
    srandom((long) getpid());

    // Enter the network loop.
    do
    {
        // Open the socket.
        sd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sd < 0)
        {
            perror("Failed to open the socket.");
            break;
        }

        // Set the path.
        memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sun_family = AF_UNIX;
        strcpy(serveraddr.sun_path, PLAYER_PATH);

        // Connect to the server
        rc = connect(sd, (struct sockaddr *)&serveraddr, SUN_LEN(&serveraddr));
        if (rc < 0)
        {
            perror("Failed to connect to server.");
            break;
        }

        // Send a "READY" flag to the server.
        strcpy(buffer, "READY\n");
        WriteSocket(sd, buffer);

        // Begin the Game Loop
        while(STOP == 0)
        {
            // Read for a "GO" or "STOP" flag.
            ReadSocket(sd, buffer);
            if(strcmp(buffer, "GO\n") == 0)
            {
                // Produce a random number and send it to the server.
                int random_num = random()%3;
                sprintf(buffer, "%d\n", random_num);
                WriteSocket(sd, buffer);
            }
            else if(strcmp(buffer, "STOP\n") == 0)
            {
                // Exit the loop.
                STOP = 1;
            }
        }
    }
    while (FALSE);

    if (sd != -1)
    {
        close(sd);
    }
}
