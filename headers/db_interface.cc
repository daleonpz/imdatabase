#include "db_interface.h"

/* ---------------------------------------- */ 
/*           CONSTRUCTOR                    */
/* ---------------------------------------- */ 
DBinterface::DBinterface(DB_INPUT* inputs){
    char *connparse;
    connparse = (char*) calloc(MAX_LENGTH, sizeof(char));

    sprintf(connparse,
            "dbname=%s host=%s user=%s password=%s",
            inputs->dbname,
            inputs->dbhost,
            inputs->dbuser,
            inputs->dbpassword);

    conn = PQconnectdb(connparse);
    free(connparse);
}

/* ---------------------------------------- */ 
/*            DESTRUCTOR                    */
/* ---------------------------------------- */ 
DBinterface::~DBinterface(){
    PQfinish(conn);
    puts("Connection is closed!");

}

/* ---------------------------------------- */ 
/*      Macros -  Templates - Functions     */
/* ---------------------------------------- */ 
template<typename T> void printElement(T t)
{
    cout << left << setw(20) << setfill(' ') << t;
}

/* ---------------------------------------- */ 
/*            M E T H O D S                 */
/* ---------------------------------------- */ 
void DBinterface::checkconnection() {
    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stderr, "Connection to database failed: %s",
                PQerrorMessage(conn));
        exit(1);
     }
     else{
        puts("Connection is established!") ;
     }
}



int DBinterface::display_menu() {

    int option;

    cout << "Que quieres hacer?" << endl;
    cout << "(1) mostrar toda la data" << endl;
    cout << "(2) buscar datos usando el codigo del alumno" << endl;
    cout << "(3) buscar datos usando el correo del alumno" << endl;
    cout << "(4) actualizar los datos de un alumno" << endl;
    cout << "(5) borrar datos de un alumno" << endl;

    cin >> option;

    if ((option < 1 ) || (option > 3) ){
        return 1;
    }

    switch(option){
        case 1: case 2 : case 3:
            retrieve_data(option-1);
            break;

        case 4:
            break;
    }



    return 0;
}



void DBinterface::retrieve_data(int qtype) {
    PGresult *res;
    string query, codigo, email;
    int ntuples;
    int nfields;
    int i,j;

  //  cout << "Table name: ";
  //  cin >> query;

    switch(qtype){
        case 0:
            query = "select * from parkour";
            break;
    
        case 1:
            cout << "Ingrese codigo:" << endl;
            cin >> codigo;
            query = "select * from parkour where codigo = " + codigo;
            break;

        case 2:
            cout << "Ingrese correo:" << endl;
            cin >> email;
            query = "select * from parkour where correo = " + email;
            break;
    }

    res = PQexec(conn, query.c_str() );
    
    if ( PQresultStatus(res) != PGRES_TUPLES_OK ){
        fprintf(stderr, "FETCH ALL failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit(1);
    }

    ntuples = PQntuples(res);
    nfields = PQnfields(res);
   
    for (j=0; j<nfields; j++) {
        printElement( PQfname(res,j) );  
        cout << "|";  
         }
    puts("");
    cout << string(62,'-') << endl;

    for (i=0; i<ntuples; i++) {
        for (j=0; j<nfields; j++) {
            printElement( PQgetvalue(res, i, j) ) ;
            cout << "|";
         }
        puts("");
    }
 
    PQclear(res);  
}

void DBinterface::update_data() {
    PGresult *res;
/*
    char c[] = "*"; 
    retrieve_data(c,0);

    cout << "what do you want to update:" << endl;
    string field;
    cin >> field;
*/

}
