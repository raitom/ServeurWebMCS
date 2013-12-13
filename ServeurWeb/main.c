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
    char *temp;
    char *fichier = NULL;
    int i = 0;

    //Requête HTTP
    eclille_receiveFromSocket(sock, buffer, sizeof(buffer));
    

    //Nom du fichier à ouvrir
    temp = buffer + 4;
    fichier = strtok(temp, " ");
    
    fprintf(stderr, "%s\n", &fichier[0]);
    
    //Fermer la connexion et le thread
    eclille_closeConnection(sock);
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    char *server_ip = "127.0.0.1";
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
    
}
