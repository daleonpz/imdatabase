#ifndef _PARSE_H_
#define _PARSE_H_

#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <getopt.h>

#define MAX_LENGTH 200
#define MAX_LENGTH_INPUT 20

using namespace std;

typedef struct{
    const char *dbname;
    const char *dbuser;
    const char *dbhost;
    char *dbpassword;
} DB_INPUT; 

void init_dbinputs(DB_INPUT *input);
void free_dbinputs(DB_INPUT *input);

int parse_input(int argc, char **argv, DB_INPUT *inputs);

int getch();
void getpass(char **pass);


#endif
 
