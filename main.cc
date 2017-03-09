#include <stdio.h>
//#include <libpq-fe.h>
#include <stdlib.h>
#include "parse.h"
#include "db_interface.h"

/*
 * TODO: 
 *      - add predefined queries ( display full table, one entry)
 *      - add data
 *      - delete data
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