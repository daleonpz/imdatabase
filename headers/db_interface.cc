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
    cout << "(3) agregar una entrada nueva" << endl;
    cout << "(4) borrar datos de un alumno" << endl;

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

        case 3:
            add_data();
            break;

        case 4:
            delete_data();
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

    cout << "\nQue desea hacer?" << endl;
    cout << "(0) mostrar toda la tabla" << endl;
    cout << "(1) mostrar una columna en particular" << endl;
    cout << "(2) buscar datos usando el codigo del alumno" << endl;
    cout << "(3) buscar datos usando el correo del alumno" << endl;
    cout << "(4) buscar datos usando el nombre del alumno" << endl;

    cin >> qtype;

    switch(qtype){
        case 0:
            query = "select * from parkour";
            break;

        case 1:
            cout << "Ingrese columna a mostrar:" << endl;
            cin >> field;
            query = "select " + field + " from parkour";
            break;
    
        case 2:
            cout << "Ingrese codigo:" << endl;
            cin >> field;
            query = "select * from parkour where codigo = " + field;
            break;

        case 3:
            cout << "Ingrese correo:" << endl;
            cin >> field;
            query = "select * from parkour where correo = '" + field + "'";
            break;
        
        case 4:
            cout << "Ingrese nombre:" << endl;
            cin.ignore();
            getline(cin,field);
            query = "select * from parkour where nombre = '" + field + "'";
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
    cout << string(nfields*21,'-') << endl;

    for (i=0; i<ntuples; i++) {
        for (j=0; j<nfields; j++) {
            printElement( PQgetvalue(res, i, j) ) ;
            cout << "|";
         }
        puts("");
    }
 
    PQclear(res); 
    return(  to_string(qtype) + field ); 
}

void DBinterface::update_data() {
    PGresult *res;
    string field;
    string op;
    string query;
    string oldvalue;
    string newvalue;

    oldvalue = retrieve_data();
    if(oldvalue.compare("0")==0) exit(0);
    
    cout << "Esta seguro de cambiar esa data? [s/n]" <<endl;
    cin >> op;

    if (op.compare("n")==0)
        exit(0);
    if (op.compare("s")!=0 ){
        cout << "opcion invalida" << endl;
        exit(1);
    }

    cout << "Que es lo que desea cambiar? [codigo/correo/nombre]" << endl;
    cin >> field;
   
    if (!(  field.compare("codigo")==0 ||
            field.compare("nombre")==0 || 
            field.compare("correo")==0 )
            ){
        cout << "opcion invalida" << endl;
        exit(1);
    } 

    switch(oldvalue.at(0)) {
            case '1':
                query = "codigo = " ; 
                oldvalue.erase(0,1);
                query += oldvalue;
                break;
            case '2':
                query = "correo = '"; 
                oldvalue.erase(0,1); 
                query += oldvalue + "'";
                break;
            case '3':
                query = "nombre = '"; 
                oldvalue.erase(0,1);    
                query += oldvalue + "'";
            }

    cout << "\nIngrese el nuevo valor" << endl;
    cin.ignore();
    getline( cin, newvalue);

    if ( field.compare("codigo")!=0){
        newvalue = "'" + newvalue + "'";
    }

    query = "update parkour set " + field + " = " + newvalue +
                "where " + query;
   
    res = PQexec(conn, query.c_str() );  

    PQclear(res);
}

void DBinterface::add_data(){
    PGresult *res;
    string query;
    string data;

    query = "insert into parkour values ( ";

    cout << "Ingrese codigo del alumno" << endl;
    cin >> data;

    if (data.length() != 8) {
        cout << "Debe ingresar 8 digitos" << endl;
        exit(1);
    }

    query += ( data + " , ");

    cout << "Ingrese nombre del alumno" << endl;
    cin.ignore();
    getline(cin, data);
    
    query += ( "'" + data + "' , ");

    cout << "Ingrese correo del alumno" << endl;
    cin >> data;

    query += ( "'" + data + "' )" ) ;    

    res = PQexec(conn, query.c_str() ) ;
    PQclear(res);
}

void DBinterface::delete_data() {
    PGresult *res;
    string op;
    string query;
    string oldvalue;

    oldvalue = retrieve_data();
    if(oldvalue.compare("0")==0) exit(0);
    
    cout << "Esta seguro de borrar esa data? [s/n]" <<endl;
    cin >> op;

    if (op.compare("n")==0)
        exit(0);
    if (op.compare("s")!=0 ){
        cout << "opcion invalida" << endl;
        exit(1);
    }

    switch(oldvalue.at(0)) {
            case '1':
                query = "codigo = " ; 
                oldvalue.erase(0,1);
                query += oldvalue;
                break;
            case '2':
                query = "correo = '"; 
                oldvalue.erase(0,1); 
                query += oldvalue + "'";
                break;
            case '3':
                query = "nombre = '"; 
                oldvalue.erase(0,1);    
                query += oldvalue + "'";
            }


    query = "delete from parkour where " + query;
   
    res = PQexec(conn, query.c_str() );  

    PQclear(res);
}
