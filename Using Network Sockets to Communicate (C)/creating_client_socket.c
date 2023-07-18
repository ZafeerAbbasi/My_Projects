/*
# ##############################################################################
# File: creating_client_socket.c                                               #
# Project: Test_files                                                          #
# Created Date: Sunday, July 16th 2023, 6:21:29 pm                             #
# Author: Zafeer Abbasi                                                        #
# ----------------------------------------------                               #
# Last Modified: Monday, July 17th 2023, 12:31:02 am                           #
# Modified By: Zafeer Abbasi                                                   #
# ----------------------------------------------                               #
# Copyright (c) 2023 Zafeer.A                                                  #
# ----------------------------------------------                               #
# HISTORY:                                                                     #
# Date      	By          	Comments                                       #
# ##########	########    	################################################
 */


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>

/*Function Declarations*/
int create_socket(void);
int socketConnect(int hSocket);
int socketSend(int hSocket, char *rqst, int rqstlen);
int socketReceive(int hSocket, char *Rsp, int RvcSize);

int main(void)
{
    int hSocket = 0, read_size = 0;
    struct sockaddr_in server = {0};
    char sendToServer[500] = {0};
    char server_reply[500] = {0};
    
    /*Create socket*/
    hSocket = create_socket();
    if(hSocket == -1)
    {
        printf("Could not create socket\n");
        return 1;
    }

    printf("Socket is created successfully\n");
    
    /*Connect to remote server*/
    if(socketConnect(hSocket) < 0)
    {
        printf("Error: Connection failed\n");
        return 1;
    }

    printf("Successfully connected to server\n");

    while(1)
    {
        printf("Client Message: ");
        fgets(sendToServer, sizeof(sendToServer), stdin);

        /*If the message contains a \n (newline) then find it and change it to null*/
        /*strchr returns a pointer to where it found the newline*/
        char *newline = strchr(sendToServer, '\n');

        /*If the pointer is not null, means theres a newline in our message*/
        /*Since newline will be at the end of message, we can just replace
        it with a null character*/
        if (newline) *newline = '\0';

        /*Send to server*/
        socketSend(hSocket, sendToServer, strlen(sendToServer));

        if(strcmp(sendToServer, "end") == 0)
        {
            printf("Closing Connection...\n");
            close(hSocket);
            return 1;
        }
        
        /*Recieve from server*/
        memset(server_reply, '\0',sizeof(server_reply));
        memset(sendToServer, '\0',sizeof(sendToServer));
        read_size = socketReceive(hSocket, server_reply, 200);
        printf("Server Message: %s\n", server_reply);

        if(strcmp(server_reply, "end") == 0)
        {
            printf("Closing connection...\n");
            close(hSocket);
            return 0;
        }
    }
}

int create_socket(void)
{
    int hSocket;
    printf("Creating the socket...\n");
    hSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    return hSocket;
}

int socketConnect(int hSocket)
{
    int iRetval = -1;
    int ServerPort = 2001;

    struct sockaddr_in remote = {0};
    remote.sin_addr.s_addr = inet_addr("127.0.0.1"); /*Local host address*/
    remote.sin_family = AF_INET;
    remote.sin_port = htons(ServerPort);

    iRetval = connect(hSocket, (struct sockaddr *)&remote, sizeof(remote));
    return iRetval;
}

int socketSend(int hSocket, char *rqst, int rqstlen)
{
    int shortRetval = -1;
    struct timeval tv;
    tv.tv_sec = 20; /*20 seconds timeout*/
    tv.tv_usec = 0;

    if(setsockopt(hSocket, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) < 0)
    {
        printf("Time out error send\n");
        return -1;
    }

    shortRetval = send(hSocket, rqst, rqstlen, 0);
    return shortRetval;
}

int socketReceive(int hSocket, char *Rsp, int RvcSize)
{
    int shortRetval = -1;
    struct timeval tv;
    tv.tv_sec = 20; /*20 seconds timeout*/
    tv.tv_usec = 0;

    if(setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
    {
        printf("Time out error recieve\n");
        return -1;      
    }

    shortRetval = recv(hSocket, Rsp, RvcSize, 0);
    return shortRetval;
}