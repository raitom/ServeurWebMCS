//
//  main.c
//  ServeurWeb
//
//  Created by Thomas Geselle on 09/12/2013.
//  Copyright (c) 2013 Thomas Geselle. All rights reserved.
//

#include <stdio.h>
#include "eclille.h"
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include "lib.h"


void *clientThreadFunction(void *data){
    int sock = *(int *)data;
    char buffer[1023];
    
    while(1){
        //Requête HTTP
        eclille_receiveFromSocket(sock, buffer, sizeof(buffer));
        printf("%s", buffer);
        
        //Récupération du fichier demandé
        char *lignes =  strtok(buffer, "\r\n");
        while (lignes != NULL) {
            printf("%s", lignes);
        }
                
        eclille_closeConnection(sock);
        pthread_exit(NULL);
    }
}

int main(int argc, char* argv[])
{
    char *server_ip = "0.0.0.0";
    int server_port = 8081;
    
    int sock = eclille_createSocket();
    
    eclille_startServer(sock, server_ip, server_port);
    
    
    while (1){
        int client_sock = eclille_waitConnection(sock);
        pthread_t thr;
        int net = pthread_create(&thr, NULL, clientThreadFunction, &client_sock);
        if (net != 0){
            printf("ERREUR DE CREATION DU THREAD");
        }
    };
    
    
    
    return 0;
}
