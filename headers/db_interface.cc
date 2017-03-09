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
    cout << "(1) mostrar datos" << endl;
    cout << "(2) actualizar los datos de un alumno" << endl;
    cout << "(3) borrar datos de un alumno" << endl;

    cin >> option;

    if ((option < 1 ) || (option > 3) ){
        return 1;
    }

    switch(option){
        case 1: 
            retrieve_data();
            break;

        case 2:
            update_data();
            break;
    }

    return 0;
}



string DBinterface::retrieve_data() {
    PGresult *res;
    string query, field;
    int qtype;
    int ntuples;
    int nfields;
    int i,j;

  //  cout << "Table name: ";
  //  cin >> query;
    cout << "\nQue desea hacer?" << endl;
    cout << "(0) mostrar toda la tabla" << endl;
    cout << "(1) buscar datos usando el codigo del alumno" << endl;
    cout << "(2) buscar datos usando el correo del alumno" << endl;
    cout << "(3) buscar datos usando el nombre del alumno" << endl;

    cin >> qtype;

    switch(qtype){
        case 0:
            query = "select * from parkour";
            break;
    
        case 1:
            cout << "Ingrese codigo:" << endl;
            cin >> field;
            query = "select * from parkour where codigo = " + field;
            break;

        case 2:
            cout << "Ingrese correo:" << endl;
            cin >> field;
            query = "select * from parkour where correo = " + field;
            break;
        
        case 3:
            cout << "Ingrese nombre:" << endl;
            cin.ignore();
            getline(cin,field);
            query = "select * from parkour where nombre = " + field;
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
    return(field); 
}

void DBinterface::update_data() {
    PGresult *res;
    string field;
    string op;
    string query;
    string oldvalue;
    string newvalue;

    oldvalue = retrieve_data();
    
    cout << "Esta seguro de cambiar esa data? [s/n]" <<endl;
    cin >> op;

    if (op.compare("n"))
        exit(1);
    if (~op.compare("n") && ~op.compare("s") ){
        cout << "opcion invalida" << endl;
        exit(1);
    }

    cout << "Que es lo que desea cambiar? [codigo/correo/nombre]" << endl;
    cin >> field;
   
    if (~op.compare("codigo") && ~op.compare("nombre") && ~op.compare("correo") ){
        cout << "opcion invalida" << endl;
        exit(1);
    } 
  /*  switch (field.c_str()){
        case "codigo": case "nombre": case "correo":
            break;
        case default:
            cout << "campo invalido" << endl;
            exit(1);
    }
*/
    query = "update parkour set " + field + " = " + oldvalue +
                     " where " + field + " = " + newvalue;
    
    /*
    char c[] = "*"; 
    retrieve_data(c,0);

    cout << "what do you want to update:" << endl;
    string field;
    cin >> field;
*/

}
