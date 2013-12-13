//
//  lib.h
//  ServeurWeb
//
//  Created by Thomas Geselle on 09/12/2013.
//  Copyright (c) 2013 Thomas Geselle. All rights reserved.
//

#ifndef ServeurWeb_lib_h
#define ServeurWeb_lib_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "eclille.h"

void nomFichier(char *adresseVariableFichier, char *buffer);
bool fichierExiste(char *nomFichier);
char *cheminFichier(char *nomFichier);
void send_not_found(int s);
void send_file(int s, const unsigned char *data, const char* filename);

#endif
