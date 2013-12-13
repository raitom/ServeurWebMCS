//
//  lib.c
//  ServeurWeb
//
//  Created by Thomas Geselle on 09/12/2013.
//  Copyright (c) 2013 Thomas Geselle. All rights reserved.
//

#include "lib.h"

void nomFichier(char *adresseVariableFichier, char *buffer)
{
    char *temp;
    char *fichier;
    
    fichier = malloc(sizeof(buffer));
    temp = buffer + 4;
    fichier = strtok(temp, " ");
    fichier = &fichier[0];
    if (strcmp(&fichier[0], "/") == 0) {
        fichier = "/index.html";
    }
    fichier = fichier + 1;
    
    strcpy(adresseVariableFichier, fichier);
}

char *cheminFichier(char *nomFichier)
{
    char *chemin = malloc(strlen(nomFichier));
    sprintf(chemin, "/Users/tgeselle/Documents/ServeurWebMCS/ServeurWeb/www/%s", nomFichier); //Concaténation pour obtenir le chemin du fichier
    return chemin;
}

bool fichierExiste(char *nomFichier)
{
    FILE *fichier = fopen(cheminFichier(nomFichier), "r+");

    if (fichier != NULL)
    {
        fclose(fichier);
        fprintf(stderr, "Le fichier existe\n");
        return true;
    }else{
        fprintf(stderr, "Le fichier n'existe pas\n");
        return false;
    }
}

void send_not_found(int s)
{
    eclille_sendToSocket(s, "HTTP/1.1 404 Not Found\r\nDate: Mon Nov 30 14:38:22 2009\r\nContent-length: 210\r\nConnection: close\r\n\r\n<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\r\n<html><head>\r\n<title>404 Not Found</title>\r\n</head><body>\r\n<h1>Not Found</h1>\r\n<p>The requested URL was not found on this server.</p>\r\n<hr>\r\n</body></html>");
}


void send_file(int s, const unsigned char *data, const char* filename)
{
}






