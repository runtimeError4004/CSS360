#ifndef DB_INSERT_H
#define DB_INSERT_H

#include <string>

// Vault writes / reads (encrypted with masterPassword)
void SQL_vaultWriter(const std::string& website,
                     const std::string& username,
                     const std::string& password);
void SQL_vaultReader();
void SQL_vaultSearch       (const std::string searchKey);
void SQL_vaultSearchDelete (const std::string searchKey);


// Access-log writes / reads
void SQL_attemptWriter(bool accessGranted);
void SQL_attemptReader();


// Utilities
int  SQL_vaultCounter();
bool deleteData();

#endif // DB_INSERT_H
