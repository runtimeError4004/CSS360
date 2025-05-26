#include <iostream>
using namespace std;
#include "sqlite3.h"

/////////////

//     Some functions source code from https://www.geeksforgeeks.org/sql-using-c-c-and-sqlite/ and will be given a citation at the 

    sqlite3* db;
    char* ErrCode;
    int SQL_QUERY = sqlite3_open("db_data.db", &db);

/*
TWO SQL database tables needed

CREDENTIALS LOGIN
-----
+ string Website
+ string Username
+ string Password

ACCESS LOG
-----
+ bool Valid
+ string Date
+ string Time

*/

/////////////

//     This function comes DIRECTLY from https://www.geeksforgeeks.org/sql-using-c-c-and-sqlite/
static int callback(void* data, int argc, char** argv, char** azColName)
{
    int i;
    fprintf(stderr, "%s: ", (const char*)data);

    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");
    return 0;
}

/*
    @brief writes the user's website, username and password to the SQL database
    

    @param string website name
    @param string username
    @param string password

*/
void SQL_vaultWriter(string website, string username, string password){
    string query = "INSERT INTO CREDENTIAL (Website, Username, Password) VALUES ('" + 
    website + "', '" + username+"', '"+ password+"');";

    SQL_QUERY = sqlite3_exec(db, query.c_str(), NULL, 0, NULL);

    if (SQL_QUERY != SQLITE_OK) {
        cerr << "Error executing WRITE statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
    } else {
        cout << "DEV - Credential STORED." << endl;
    }

}


/*
    @brief reads the password vault database
*/
void SQL_vaultReader(){
    string query = "SELECT * FROM CREDENTIAL;";

    SQL_QUERY = sqlite3_exec(db, query.c_str(), callback, (void*)"CREDENTIAL", NULL); 

    if (SQL_QUERY != SQLITE_OK) {
        std::cerr << "Error executing READING statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
    }


    // cout<<"\n/////function incomplete/////\n";

}


/*
    @brief inserts date, time, and valid entry for every attempt to enter the vault
    
    @param bool on whether the masterpassword enetered was valid

    @return null
*/
void SQL_attemptWriter(bool accessGranted){
    string date; // get date
    string time; // get date

    // database function. Insert date, time, accessGranted

    cout<<"\n/////function incomplete/////\n";
}


/*
    @brief reads the list of all vault master password attempts from SQL DB
    
*/
void SQL_attemptReader(){


    cout<<"\n/////function incomplete/////\n";

}

int SQL_vaultCounter(){
    string query = "SELECT COUNT(*) FROM CREDENTIAL;";

    SQL_QUERY = sqlite3_exec(db, query.c_str(), NULL, 0, NULL);
    if (SQL_QUERY != SQLITE_OK) {
        std::cerr << "Error executing COUNTING statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
    } else {
        std::cout << "All records deleted from 'users' table:" << std::endl;

    }

    
    return false;
}

bool deleteData(){
    string query = "DELETE FROM CREDENTIAL;";

    SQL_QUERY = sqlite3_exec(db, query.c_str(), NULL, 0, NULL);
    if (SQL_QUERY != SQLITE_OK) {
        std::cerr << "Error executing READING statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
    } else {
        std::cout << "All records deleted from 'users' table:" << std::endl;

    }

    
    return false;
}