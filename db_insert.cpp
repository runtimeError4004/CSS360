#include <iostream>
#include <string>
#include <ctime>
#include "db_insert.h"
#include "aes256.h"
#include "master_password.h"
#include <sqlite3.h>

using namespace std;
extern sqlite3* db;

// Helper: given a JSON of the form {"site":"…","user":"…","pass":"…"}, extract
// the value for a given key. Returns empty string if not found.
static string extractJsonField(const string &json, const string &key) {
    // We look for:  "<key>":"<value>"
    string pattern = "\"" + key + "\":\"";
    size_t start = json.find(pattern);
    if (start == string::npos) return "";

    start += pattern.size();
    size_t end = json.find('"', start);
    if (end == string::npos) return "";

    return json.substr(start, end - start);
}

// Inserts a new credential record, encrypting it with AES-256 and the masterPlain.
void SQL_vaultWriter(const string& website,
                     const string& username,
                     const string& password)
{
    // Build JSON payload
    string payload = "{\"site\":\"" + website + "\"," 
                   + "\"user\":\"" + username + "\"," 
                   + "\"pass\":\"" + password + "\"}";

    // Encrypt payload using AES-256 with masterPlain as the key
    string blob = aes256(true, masterPlain, payload);

    const char* sql = 
        "INSERT INTO CREDENTIAL (Website, Username, Password) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "[ERROR] Failed to prepare write stmt: " 
             << sqlite3_errmsg(db) << "\n";
        return;
    }
    sqlite3_bind_text(stmt, 1, website.c_str(),  -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, blob.c_str(),     -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "[ERROR] INSERT failed: " << sqlite3_errmsg(db) << "\n";
    }
    sqlite3_finalize(stmt);
}

// Reads and decrypts all credential records, printing only Website, Username, and Password
void SQL_vaultReader()
{
    const char* sql = "SELECT Website, Username, Password FROM CREDENTIAL;";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "[ERROR] Failed to prepare read: " << sqlite3_errmsg(db) << "\n";
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // 1) Plain‐text columns:
        string site = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        string user = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

        // 2) Encrypted JSON blob (hex) stored in column 2:
        const char* blobText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        if (!blobText) {
            // Skip if somehow null
            continue;
        }
        string hexBlob(blobText);

        // 3) Decrypt to JSON: {"site":"…","user":"…","pass":"…"}
        string json;
        try {
            json = aes256(false, masterPlain, hexBlob);
        } catch (const exception &e) {
            cerr << "[ERROR] Decrypt failed: " << e.what() << "\n";
            continue;
        }

        // 4) Extract each field
        string parsedSite = extractJsonField(json, "site");
        string parsedUser = extractJsonField(json, "user");
        string parsedPass = extractJsonField(json, "pass");

        // 5) Print only what’s requested:
        cout << "Website:  " << parsedSite  << "\n"
             << "Username: " << parsedUser  << "\n"
             << "Password: " << parsedPass  << "\n\n";
    }

    sqlite3_finalize(stmt);
}



bool SQL_vaultSearch(std::string searchKey)
{
    const char* sql = 
      "SELECT Website, Username, Password "
      "FROM CREDENTIAL "
      "WHERE Website = ? OR Username = ?;";
    sqlite3_stmt* stmt = nullptr;

    bool recordsFound;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "[ERROR] Failed to prepare search: " 
             << sqlite3_errmsg(db) << "\n";
        recordsFound = false;
    }

    sqlite3_bind_text(stmt, 1, searchKey.c_str(), searchKey.size(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, searchKey.c_str(), searchKey.size(), SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string site = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        string user = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* blobText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        if (!blobText) {
            continue;
        }
        string hexBlob(blobText);

        string json;
        try {
            json = aes256(false, masterPlain, hexBlob);
        } catch (const exception &e) {
            cerr << "[ERROR] Decrypt failed: " << e.what() << "\n";
            continue;
        }

        string parsedSite = extractJsonField(json, "site");
        string parsedUser = extractJsonField(json, "user");
        string parsedPass = extractJsonField(json, "pass");

        cout << "Website:  " << parsedSite  << "\n"
             << "Username: " << parsedUser  << "\n"
             << "Password: " << parsedPass  << "\n\n";

        recordsFound = true;
    }

    sqlite3_finalize(stmt);
    return recordsFound;
}

void SQL_vaultSearchUpdate (string searchKey){
    // scrap
};

void SQL_vaultSearchDelete (string searchKey){
    
    const char* sql = 
      "DELETE FROM CREDENTIAL "
      "WHERE Website = ? OR Username = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "[ERROR] Failed to prepare search: " 
             << sqlite3_errmsg(db) << "\n";
        return;
    } else {
        cout<< "Deleted all entries.\n";
    }

    sqlite3_bind_text(stmt, 1, searchKey.c_str(), searchKey.size(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, searchKey.c_str(), searchKey.size(), SQLITE_TRANSIENT);


    sqlite3_finalize(stmt);
};

// Logs each vault access attempt (valid or not) with timestamp
void SQL_attemptWriter(bool accessGranted)
{
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char date[11];
    char timeStr[9];
    strftime(date,    sizeof(date),    "%Y-%m-%d", ltm);
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", ltm);

    string query = 
      "INSERT INTO ACCESS_LOG (Valid, Date, Time) VALUES ('" +
      string(accessGranted ? "1" : "0") + "', '" +
      date + "', '" + timeStr + "');";
    char* errMsg = nullptr;
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        cerr << "[ERROR] Access log write failed: " << errMsg << "\n";
        sqlite3_free(errMsg);
    }
}

// Reads and prints all access log entries
void SQL_attemptReader()
{
    const char* sql = "SELECT Valid, Date, Time FROM ACCESS_LOG;";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "[ERROR] Failed to prepare attempt read: " 
             << sqlite3_errmsg(db) << "\n";
        return;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int valid = sqlite3_column_int(stmt, 0);
        const char* date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* time = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        cout << "Attempt - Valid: " << valid 
             << ", Date: " << date 
             << ", Time: " << time << "\n";
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
        cerr << "[ERROR] Failed to prepare count: " << sqlite3_errmsg(db) << "\n";
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
        cerr << "[ERROR] deleteData failed: " << errMsg << "\n";
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}
