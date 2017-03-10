#include <stdio.h>
//#include <libpq-fe.h>
#include <stdlib.h>
#include "headers/db_interface.h"

/*
 * TODO: 
 * */


int main(int argc, char* argv[]) {
 
    DB_INPUT dbinputs;
    init_dbinputs(&dbinputs);
    parse_input(argc,argv,&dbinputs);

    DBinterface dbinterface(&dbinputs);
    free_dbinputs(&dbinputs);

    dbinterface.checkconnection();

    if( dbinterface.display_menu() < 0 ){
        puts("invalid option");
        exit(1);
    } 

     return 0;
 }
