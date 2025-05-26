#include <iostream>
using namespace std;
#include "sqlite3.h"

extern sqlite3* db;
// 
/////////////

//     Some functions source code from https://www.geeksforgeeks.org/sql-using-c-c-and-sqlite/ and will be given a citation at the 

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

    int SQL_QUERY = sqlite3_exec(db, query.c_str(), NULL, 0, NULL);

    if (SQL_QUERY != SQLITE_OK) {
        cerr << "DEVNOTE - Error executing WRITE statement: " << sqlite3_errmsg(db) << std::endl;
    } else {
        // cout << "DEVNOTE - Credential STORED." << endl;
    }

}


/*
    @brief reads the password vault database
*/
void SQL_vaultReader(){
    string query = "SELECT * FROM CREDENTIAL;";

    int SQL_QUERY = sqlite3_exec(db, query.c_str(), callback, (void*)"CREDENTIAL", NULL); 

    if (SQL_QUERY != SQLITE_OK) {
        std::cerr << "DEVNOTE - Error executing READING statement: " << sqlite3_errmsg(db) << std::endl;
    }
    else {
        // cout << "DEVNOTE - Credential STORED." << endl;
    }
<<<<<<< Updated upstream
=======
    std::cout << "All records deleted from 'users' table." << std::endl;

    sqlite3_exec(db, query.c_str(), callback, NULL, NULL);
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes

}

/*
    @brief inserts date, time, and valid entry for every attempt to enter the vault
    
    @param bool on whether the masterpassword enetered was valid

    @return null
*/
void SQL_attemptWriter(bool accessGranted){
    // Get current date and time
    // This appears to be the easiest way to format Date and Time... thanks, C++!
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char date[11]; // YYYY-MM-DD
    char timeStr[9]; // HH:MM:SS
    strftime(date, sizeof(date), "%Y-%m-%d", ltm);
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", ltm);

<<<<<<< Updated upstream
    string query = "INSERT INTO ACCESS_LOG (Valid, Date, Time) VALUES ('" + string(accessGranted ? "1" : "0") + "', '" + date + "', '" + timeStr + "');";

    int SQL_QUERY = sqlite3_exec(db, query.c_str(), NULL, 0, NULL);

    if (SQL_QUERY != SQLITE_OK) {
        cerr << "Error executing WRITE statement: " << sqlite3_errmsg(db) << std::endl;
    }
=======
    // database function. Insert date, time, accessGranted
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
}


/*
    @brief reads the list of all vault master password attempts from SQL DB
    
*/
void SQL_attemptReader(){
    // basically copy/paste of SQL_vaultReader but from a different table...
    string query = "SELECT * FROM ACCESS_LOG;";
    // string query = "SELECT Valid, Date, Time FROM ACCESS_LOG;";


    int SQL_QUERY = sqlite3_exec(db, query.c_str(), callback, (void*)"ACCESS_LOG", NULL); 

    if (SQL_QUERY != SQLITE_OK) {
        std::cerr << "Error executing READING statement: " << sqlite3_errmsg(db) << std::endl;
    }
}

// havent set this up yet
int SQL_vaultCounter(){
    string query = "SELECT COUNT(*) FROM CREDENTIAL;";

    int SQL_QUERY = sqlite3_exec(db, query.c_str(), NULL, 0, NULL);
    if (SQL_QUERY != SQLITE_OK) {
        std::cerr << "Error executing COUNTING statement: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "All records deleted from 'users' table:" << std::endl;

    }
    
    return 0;
}

bool deleteData(){
    string query = "DELETE FROM CREDENTIAL;";

    int SQL_QUERY = sqlite3_exec(db, query.c_str(), NULL, 0, NULL);
    if (SQL_QUERY != SQLITE_OK) {
        std::cerr << "Error executing DELETE statement: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "All records deleted from 'users' table:" << std::endl;

    }

    
    return false;
}