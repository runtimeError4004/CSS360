#include <iostream>
using namespace std;
//#include <sqlite3.h>

int main()
{


    /*
    A lot of material came from:
    https://www.geeksforgeeks.org/sql-using-c-c-and-sqlite/

    I have tried my best to keep the code our own where I can. - Justin
    */
    sqlite3* DB;

///////////////////////////////////////////////////////////////
    string create_cred_table = 
    "CREATE TABLE IF NOT EXISTS CREDENTIAL ("
    "ID INT PRIMARY KEY NOT NULL, "
    "Website  TEXT    NOT NULL, "
    "Username TEXT    NOT NULL, "
    "Password TEXT     NOT NULL "
    ");";

    
    int exit = 0;
    exit = sqlite3_open("data.db", &DB);
    char* ErrCode;
    exit = sqlite3_exec(DB, create_cred_table.c_str(), NULL, 0, &ErrCode);

    if (exit != SQLITE_OK) {
        cerr << "Error Create Table" << ErrCode << "\n";
        sqlite3_free(ErrCode);
    }
    else {
        cout << "Table created Successfully\n";
    }



///////////////////////////////////////////////////////////////


    string create_log_table = 
    "CREATE TABLE IF NOT EXISTS ACCESS_LOG ("
    "LogID      INT PRIMARY KEY NOT NULL, "
    "Valid      INT     NOT NULL, "
    "Date       TEXT    NOT NULL, "
    "Time       TEXT    NOT NULL "
    ");";
    

    exit = sqlite3_open("data.db", &DB);
    exit = sqlite3_exec(DB, create_log_table.c_str(), NULL, 0, &ErrCode);

    if (exit != SQLITE_OK) {
        cerr << "Error Create Table" << ErrCode << "\n";
        sqlite3_free(ErrCode);
    }
    else {
        cout << "Table created Successfully\n";
    }




    // this comes at the end of the program
    sqlite3_close(DB);
    return (0);
}