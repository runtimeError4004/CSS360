#include <iostream>
using namespace std;
#include "sqlite3.h"

extern sqlite3* db;

int create_tables()
{

    /*
    A lot of material came from:
    https://www.geeksforgeeks.org/sql-using-c-c-and-sqlite/

    I have tried my best to keep the code our own where I can. - Justin
    */
///////////////////////////////////////////////////////////////

    extern sqlite3* db;

///////////////////////////////////////////////////////////////
    string create_cred_table = 
    "CREATE TABLE IF NOT EXISTS CREDENTIAL ("
    "ID         INTEGER     PRIMARY KEY AUTOINCREMENT, "
    "Website    TEXT        NOT NULL, "
    "Username   TEXT        NOT NULL, "
    "Password   TEXT        NOT NULL "
    ");";

    char* ErrCode = nullptr;
    int exit = sqlite3_exec(db, create_cred_table.c_str(), NULL, 0, &ErrCode);

    if (exit != SQLITE_OK) {
        cerr << "Error Create Table" << ErrCode << "\n";
        sqlite3_free(ErrCode);
    }

    string create_log_table = 
        "CREATE TABLE IF NOT EXISTS ACCESS_LOG ("
        "ID         INTEGER PRIMARY KEY AUTOINCREMENT, "
        "Valid      INTEGER NOT NULL, "
        "Date       TEXT    NOT NULL, "
        "Time       TEXT    NOT NULL "
        ");";

    exit = sqlite3_exec(db, create_log_table.c_str(), NULL, 0, &ErrCode);

    if (exit != SQLITE_OK) {
        cerr << "Error Create Table" << ErrCode << "\n";
        sqlite3_free(ErrCode);
    } else {
        // cout<<"DEVNOTE - Table created Successfully\n";
    }

///////////////////////////////////////////////////////////////

    create_log_table = 
    "CREATE TABLE IF NOT EXISTS ACCESS_LOG ("
    "ID         INTEGER PRIMARY KEY AUTOINCREMENT, "
    "Valid      INTEGER NOT NULL, "
    "Date       TEXT    NOT NULL, "
    "Time       TEXT    NOT NULL "
    ");";
    

    exit = sqlite3_exec(db, create_log_table.c_str(), NULL, 0, &ErrCode);

    if (exit != SQLITE_OK) {
        cerr << "Error Create Table" << ErrCode << "\n";
        sqlite3_free(ErrCode);
    }
    else {
        // cout<<"DEVNOTE - Table created Successfully\n";
    }

    return (0);
}

