/** ROCK PAPER SCISSORS **/
/*  BY: DAVID JUSINO	**/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "readstr.h"
#define PLAYER_PATH   "/tmp/player_server"
#define BUFFER_LENGTH    250
#define FALSE              0

void main(int argc, char *argv[])
{
    // Unlink Paths, just in case something funky happened.
    unlink(PLAYER_PATH);

    // Required Variables for both sockets.
    int  iterations, length, STOP=0;
    char buffer[BUFFER_LENGTH], buffer2[BUFFER_LENGTH];
    struct sockaddr_un serveraddr;

    // Process the arguement.
    iterations = atoi(argv[0]);

    // Make adjustments if things weren't submitted properly.
    if (iterations <= 0){
        iterations = 5;
    }

    // Universal Socket Variables.
    int plyd=-1, ply_c=-1;

    // Player One variables.
    int ply1=-1;
    int ply1_score=0;

    // Player Two variables.
    int ply2=-1;
    int ply2_score=0;

    // Enter the network loop.
    do
    {
        // Open the Socket for both players.
        plyd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (plyd < 0)
        {
            perror("Socket failed to open.");
            break;
        }

        // Bind to the sockets.
        // Bind the path for the first player.
        memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sun_family = AF_UNIX;
        strcpy(serveraddr.sun_path, PLAYER_PATH); //

        ply_c = bind(plyd, (struct sockaddr *)&serveraddr, SUN_LEN(&serveraddr));
        if (ply_c < 0)
        {
            perror("The socket for Player One failed to bind.");
            break;
        }

        // Listen on each socket for exactly one connection
        ply_c = listen(plyd, 1);

        printf("Ready for both players to connect to the server.\n");

        // Accept the first connection
        ply1 = accept(plyd, NULL, NULL);
        if (ply1 < 0)
        {
            perror("Player one failed to accept the connection to the server.");
            break;
        }

        // Accept the second connection
        ply2 = accept(plyd, NULL, NULL);
        if (ply2 < 0)
        {
            perror("Player two failed to accept the connection to the server.");
            break;
        }

        // Stop listening on this socket now that both players are connected.
        if (plyd != -1)
        {
            close(plyd);
        }

        // Wait for the "READY" flag from each player.
        ReadSocket(ply1, buffer);
        ReadSocket(ply2, buffer2);

        // Enter the Game.
        while(STOP == 0)
        {
            // Check that both players are ready.
            if(strcmp(buffer, "READY\n") == 0, strcmp(buffer2, "READY\n") == 0)
            {
                printf("Both players are ready!\nBeginning the game!\n\n");
                int i;

                // Start of For Loop
                for(i = 0; i < iterations; i++)
                {
                    // Send each player a "GO" flag.
                    WriteSocket(ply1, "GO\n");
                    WriteSocket(ply2, "GO\n");

                    // Wait on each player's response with their choice.
                    ReadSocket(ply1, buffer);
                    ReadSocket(ply2, buffer2);

                    // Create the variables to hold the strings and values for each player.
                    int player1, player2;
                    char *plystr, *plystr2;

                    // Transfer the string values in the buffer to an integer.
                    player1 = atoi(buffer);
                    player2 = atoi(buffer2);

                    /// GAME LEGEND:
                    // ROCK = 0, PAPER = 1, SCISSORS = 2
                    // ROCK BEATS SCISSORS (0, 2)
                    // SCISSORS BEATS PAPER (2, 1)
                    // PAPER BEATS ROCK (1, 0)

                    // Assign the proper string value based on the choice.
                    if(player1 == 0)
                    {
                        plystr = "Rock";
                    }
                    else if(player1 == 1)
                    {
                        plystr = "Paper";
                    }
                    else if(player1 == 2)
                    {
                        plystr = "Scissors";
                    }

                    if(player2 == 0)
                    {
                        plystr2 = "Rock";
                    }
                    else if(player2 == 1)
                    {
                        plystr2 = "Paper";
                    }
                    else if(player2 == 2)
                    {
                        plystr2 = "Scissors";
                    }

                    // Decide who won and increment the score.
                    if (player1 == player2)
                    {
                        // Print out player choices and current points summary in parentheses.
                        printf("\n\nRound #%d -\nPlayer One (%d) picked: %s\nPlayer Two (%d) picked: %s\n", i+1, ply1_score, plystr, ply2_score, plystr2);
                        printf("Draw! Both players picked %s!\n", plystr);
                        //No Score Gain
                    }
                    else if ((player1 == 0 && player2 == 2) || (player1 == 2 && player2 == 1) || (player1 == 1 && player2 == 0))
                    {
                        // Increment the score
                        ply1_score++;

                        // Print out player choices and current points summary in parentheses.
                        printf("\n\nRound #%d -\nPlayer One (%d) picked: %s\nPlayer Two (%d) picked: %s\n", i+1, ply1_score, plystr, ply2_score, plystr2);
                        printf("Player One wins! %s beats %s!\n", plystr, plystr2);

                    }
                    else
                    {
                        // Increment the score
                        ply2_score++;

                        printf("\n\nRound #%d -\nPlayer One (%d) picked: %s\nPlayer Two (%d) picked: %s\n", i+1, ply1_score, plystr, ply2_score, plystr2);
                        printf("Player Two wins! %s beats %s!\n", plystr2, plystr);

                    }



                } // End of For Loop

                // End the Game and tell each player to stop running.
                printf("\n\nThe game has ended!\n");
                WriteSocket(ply1, "STOP\n");
                WriteSocket(ply2, "STOP\n");
                STOP=1; //To escape the while loop.
            }
        }

        // Decide the scores and announce who won.
        printf("********************\n");
        printf("    Final Scores    \nPlayer One: %d\nPlayer Two: %d\n", ply1_score, ply2_score);
        printf("********************\n");
        if(ply1_score == ply2_score)
        {
            printf("Draw! No one wins!\n");
        }
        else if (ply1_score > ply2_score)
        {
            printf("Player One wins the game!\n");
        }
        else if (ply2_score > ply1_score)
        {
            printf("Player Two wins the game!\n");
        }
    }
    while (FALSE);

    // Close down the sockets
    if (plyd != -1)
    {
        close(plyd);
    }
    if (ply1 != -1)
    {
        close(ply1);
    }
    if (ply2 != -1)
    {
        close(ply2);
    }
    // Unlink the paths
    unlink(PLAYER_PATH);
}
