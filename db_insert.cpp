#include <iostream>
#include <ctime>
#include "db_insert.h"
#include "aes256.h"
#include "master_password.h"
#include <sqlite3.h>

using namespace std;
extern sqlite3* db;

// Inserts a new credential record, encrypting it with AES-256 and the masterPassword.
void SQL_vaultWriter(const string& website,
    const string& username,
    const string& password)
{
// cout 
// // << "[DEBUG] SQL_vaultWriter called\n"
// << "  site:     " << website  << "\n"
// << "  username: " << username << "\n";

// Build JSON payload
string payload = "{\"site\":\"" + website + "\"," 
  + "\"user\":\"" + username + "\"," 
  + "\"pass\":\"" + password + "\"}";
// Encrypt payload
string blob = aes256(true, masterPlain, payload);

//cout << "[DEBUG] Encrypted payload (hex): " << blob << "\n";

const char* sql = 
"INSERT INTO CREDENTIAL (Website, Username, Password) VALUES (?, ?, ?);";
sqlite3_stmt* stmt = nullptr;
if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
cerr << "[ERROR] Failed to prepare write stmt: " << sqlite3_errmsg(db) << "\n";
return;
}
sqlite3_bind_text(stmt, 1, website.c_str(), -1, SQLITE_TRANSIENT);
sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_TRANSIENT);
sqlite3_bind_text(stmt, 3, blob.c_str(),    -1, SQLITE_TRANSIENT);

int rc = sqlite3_step(stmt);
if (rc != SQLITE_DONE) {
cerr << "[ERROR] INSERT failed: " << sqlite3_errmsg(db) << "\n";
} else {
// cout << "[DEBUG] INSERT succeeded.\n";
}
sqlite3_finalize(stmt);
}   



void SQL_vaultReader()
{
    const char* sql = "SELECT Website, Username, Password FROM CREDENTIAL;";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare read: " << sqlite3_errmsg(db) << "\n";
        return;
    }

    int row = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        row++;
        string site = (const char*)sqlite3_column_text(stmt, 0);
        string user = (const char*)sqlite3_column_text(stmt, 1);
        
        
        const char* blobText = (const char*)sqlite3_column_text(stmt, 2);
        if (!blobText) {
            cerr << "[WARN] Row " << row << " missing encrypted data\n";
            continue;
        }

        string hexBlob(blobText);
        cout << "\n[Entry " << row << "]\n"
             << "Site:     " << site << "\n"
             << "Username: " << user << "\n";
            //  << "Hex blob: " << hexBlob << "\n";
             

        try {
            string json = aes256(false, masterPlain, hexBlob);
            cout << "Password: " << json << "\n";
        } catch (const exception& e) {
            cout << "[ERROR] Decrypt failed: " << e.what() << "\n";
        }
    }

    sqlite3_finalize(stmt);
}



void SQL_vaultSearch(string searchKey)
{
    const char* sql = "SELECT Website, Username, Password FROM CREDENTIAL WHERE Website = ? OR Username = ?;";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare read: " << sqlite3_errmsg(db) << "\n";
        return;
    }

    // searching for searchKey in the Website ? placeholder
    if (sqlite3_bind_text(stmt, 1, searchKey.c_str(), searchKey.length(), SQLITE_TRANSIENT) != SQLITE_OK) {
        cerr << "Failed to prepare read: " << sqlite3_errmsg(db) << "\n";
        return;
    }
    // searching for searchKey in the Username ? placeholder
    if (sqlite3_bind_text(stmt, 2, searchKey.c_str(), searchKey.length(), SQLITE_TRANSIENT) != SQLITE_OK) {
        cerr << "Failed to prepare read: " << sqlite3_errmsg(db) << "\n";
        return;
    }

    int row = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        row++;
        string site = (const char*)sqlite3_column_text(stmt, 0);
        string user = (const char*)sqlite3_column_text(stmt, 1);
        
        
        const char* blobText = (const char*)sqlite3_column_text(stmt, 2);
        if (!blobText) {
            cerr << "[WARN] Row " << row << " missing encrypted data\n";
            continue;
        }

        string hexBlob(blobText);
        cout << "\n[Entry " << row << "]\n"
             << "Site:     " << site << "\n"
             << "Username: " << user << "\n"
             << "Hex blob: " << hexBlob << "\n";

        try {
            string json = aes256(false, masterPlain, hexBlob);
            cout << "Decrypted data: " << json << "\n";
        } catch (const exception& e) {
            cerr << "[ERROR] Decrypt failed: " << e.what() << "\n";
        }
    }

    sqlite3_finalize(stmt);
}

void SQL_vaultSearchDelete (string searchKey){

};

// Logs each vault access attempt (valid or not) with timestamp
void SQL_attemptWriter(bool accessGranted)
{
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char date[11];
    char timeStr[9];
    strftime(date, sizeof(date), "%Y-%m-%d", ltm);
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", ltm);

    string query = "INSERT INTO ACCESS_LOG (Valid, Date, Time) VALUES ('" + 
                   string(accessGranted ? "1" : "0") + "', '" + date + "', '" + timeStr + "');";
    char* errMsg = nullptr;
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        cerr << "Error executing attempt write: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

// Reads and prints all access log entries
void SQL_attemptReader()
{
    const char* sql = "SELECT Valid, Date, Time FROM ACCESS_LOG;";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "DEVNOTE - Failed to prepare attempt read: " << sqlite3_errmsg(db) << endl;
        return;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int valid = sqlite3_column_int(stmt, 0);
        const char* date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        cout << "Attempt - Valid: " << valid << ", Date: " << date << ", Time: " << time << endl;
    }
    sqlite3_finalize(stmt);
}

// Returns the count of credential records
int SQL_vaultCounter()
{
    const char* sql = "SELECT COUNT(*) FROM CREDENTIAL;";
    sqlite3_stmt* stmt = nullptr;
    int count = 0;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }
    } else {
        cerr << "DEVNOTE - Failed to prepare count: " << sqlite3_errmsg(db) << endl;
    }
    sqlite3_finalize(stmt);
    return count;
}

// Deletes all credential records
bool deleteData()
{
    const char* sql = "DELETE FROM CREDENTIAL;";
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        cerr << "DEVNOTE - Error deleting data: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}