//
//  main.h
//  ServeurWeb
//
//  Created by Thomas Geselle on 13/12/2013.
//  Copyright (c) 2013 Thomas Geselle. All rights reserved.
//

#ifndef ServeurWeb_main_h
#define ServeurWeb_main_h

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lib.h"
#include "eclille.h"

void *clientThreadFunction(void *data);
int main(int argc, char* argv[]);

#endif
