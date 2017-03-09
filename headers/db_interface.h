#ifndef _DB_INTERFACE_H_
#define _DB_INTERFACE_H_ 

#include <iostream>
#include <string>
#include <iomanip>
#include <libpq-fe.h>
#include "parse.h"


using namespace std;

/* ------------------------------------------- */
/*            DB INTERFACE                     */
/* ------------------------------------------- */

class DBinterface {
    private:
        PGconn *conn;
    public:
        DBinterface(DB_INPUT*);
        ~DBinterface();

        void checkconnection() ;
        int display_menu() ;
        string retrieve_data();
        void update_data() ;
        void add_data();
        void delete_data() ;
};
#endif
