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
    // Get current date and time
    time_t now = time(0);
    tm* ltm = localtime(&now);

    // This appears to be the easiest way to format Date and Time... thanks, C++!
    char date[11]; // YYYY-MM-DD
    char timeStr[9]; // HH:MM:SS
    strftime(date, sizeof(date), "%Y-%m-%d", ltm);
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", ltm);

    // Prepare SQL statement (this seems to be better SQLite3 practice? im just Google searching this, man)
    string sql = "INSERT INTO ACCESS_LOG (Valid, Date, Time) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, accessGranted ? 1 : 0);
        sqlite3_bind_text(stmt, 2, date, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, timeStr, -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cerr << "Failed to insert access log: " << sqlite3_errmsg(db) << endl;
        }
    } else {
        cerr << "Failed to prepare insert statement: " << sqlite3_errmsg(db) << endl;
    }
    sqlite3_finalize(stmt);
}


/*
    @brief reads the list of all vault master password attempts from SQL DB
    
*/
void SQL_attemptReader(){
    string query = "SELECT ID, Valid, Date, Time FROM ACCESS_LOG;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    cout << "Access Log Entries:\n";
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        int valid = sqlite3_column_int(stmt, 1);
        const unsigned char* date = sqlite3_column_text(stmt, 2);
        const unsigned char* time = sqlite3_column_text(stmt, 3);

        cout << "ID: " << id
             << ", Date: " << (date ? reinterpret_cast<const char*>(date) : "NULL")
             << ", Time: " << (time ? reinterpret_cast<const char*>(time) : "NULL")
             << ", Success: " << (valid == 1 ? "Yes" : "No") << endl;
    }

    sqlite3_finalize(stmt);

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