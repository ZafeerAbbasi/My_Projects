/*
# ##############################################################################
# File: creating_server_socket.c                                               #
# Project: Test_files                                                          #
# Created Date: Saturday, July 15th 2023, 10:19:21 pm                          #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Monday, July 17th 2023, 12:26:14 am                           #
# Modified By: Zafeer Abbasi                                                   #
# ----------------------------------------------                               #
# Copyright (c) 2023 Zafeer.A                                                  #
# ----------------------------------------------                               #
# HISTORY:                                                                     #
# Date      	By          	Comments                                       #
# ##########	########    	################################################
 */

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

/*Function Declarations*/
short socketCreate(void);
int bindCreatedSocket(int hSocket);


int main(int argc, char *argv[])
{
    int socket_desc = 0, sock = 0, clientLen = 0;
    struct sockaddr_in client;
    char client_message[500] = {0};
    char message[500] = {0};
    const char *pMessage = "Hello from Zafeer's server";

    /*Create socket*/
    socket_desc = socketCreate();
    if(socket_desc == -1)
    {
        printf("Could not create socket\n");
        return 1;
    }

    printf("Socket Created Successfully\n");

    /*Bind*/
    if(bindCreatedSocket(socket_desc) < 0)
    {
        printf("Bind failed\n");
        return 1;
    }

    printf("Bind has completed successfully\n");
    
    /*Begin Listening*/
    listen(socket_desc, 3);

    /*Accept incoming connections*/
    while(1)
    {
        printf("Waiting for incoming connections...\n");
        clientLen = sizeof(struct sockaddr_in);

        /*The accept call will block (Wait) until we get a connection call from client*/
        sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&clientLen);
        
        if(sock < 0)
        {
            perror("Accept Failed\n");
            return 1;
        }

        printf("Connection Accepted\n");
        

        while(1)
        {
            /*Setting arrays to NULL so that end character will be NULL after adding data*/
            memset(client_message,'\0',sizeof(client_message));
            memset(message, '\0', sizeof(message));

            /*Receive a reply from the client*/
            /*The recv function is blocking, which means it will wait at the point until it has actually received a message from the client*/
            if(recv(sock, client_message, 200, 0) < 0)
            {
                printf("Recv failed\n");
                break;
            }
            
            printf("Client Message: %s\n", client_message);
            if(strcmp("end", client_message) == 0)
            {
                printf("Server Message: Okay, bye bye!\n");
            }
            else
            {
                printf("Server Message: ");
                fgets(message, sizeof(message), stdin);

                /*If the message contains a \n (newline) then find it and change it to null*/
                /*strchr returns a pointer to where it found the newline*/
                char *newline = strchr(message, '\n');

                /*If the pointer is not null, means theres a newline in our message*/
                /*Since newline will be at the end of message, we can just replace
                it with a null character*/
                if (newline) *newline = '\0';
            }
            
            /*Send the data from server to client*/
            if(send(sock, message, strlen(message), 0) < 0)
            {
                printf("Send from server to client failed\n");
                return 1;
            }

            if(strcmp(message, "end") == 0)
            {
                printf("Closing server connection...\n");
                close(sock);
                return 0;
            }
        }

        //close(sock);
        sleep(1);
    }
}




/*Function to create socket*/
short socketCreate(void)
{

    /*Create a place holder for the file descritor/handle returned by socket function*/
    short hSocket;
    
    printf("Creating the socket...\n");
    
    /*Creating the socket*/
    hSocket = socket(AF_INET, SOCK_STREAM, 0);

    /*Returning the file descriptor*/
    return hSocket;
}

int bindCreatedSocket(int hSocket)
{
    int iRetval = -1;
    
    /*Setting the client port*/
    int clientPort = 2001;

    /*Declaring our sockaddr_in struct and initializing it to zero*/
    struct sockaddr_in remote = {0};

    /*Internet address family*/
    remote.sin_family = AF_INET;

    /*Any incoming interface*/
    /*Using htonl == Host to Network Long, to ensure the byte order from host data can be used in Network in case they are different*/
    remote.sin_addr.s_addr = htonl(INADDR_ANY); //set incoming to any address
    
    /*Using htons == Host to Network Short to ensure the byte order from host data can be used in Network in case they are different*/
    remote.sin_port = htons(clientPort); /*Local port*/
    
    /*Calling the bind function*/
    /*In memory, the struct sockaddr_in is the same as sockaddr, sockaddr_in is just a version of sockaddr that is specifically designed to handle internet addresses*/
    iRetval = bind(hSocket, (struct sockaddr *)&remote, sizeof(remote));
    
    return iRetval;    
}

