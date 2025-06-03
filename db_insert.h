#ifndef DB_INSERT_H
#define DB_INSERT_H

#include <string>
using namespace std;    

// Vault writes / reads (encrypted)
void SQL_vaultWriter(const string& website,
                     const string& username,
                     const string& password);
void SQL_vaultReader();

bool SQL_vaultSearch       (const std::string searchKey);
// void SQL_vaultSearchUpdate (const std::string searchKey); // scrapped
void SQL_vaultSearchDelete (const std::string searchKey);

// Access-log writes / reads
void SQL_attemptWriter(bool accessGranted);
void SQL_attemptReader();

// Utilities
int  SQL_vaultCounter();
bool deleteData();

#endif // DB_INSERT_H
