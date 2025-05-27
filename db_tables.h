#ifndef DB_TABLES_H
#define DB_TABLES_H

// Ensures the necessary database tables exist (CREDENTIAL and ACCESS_LOG).
// Returns SQLITE_OK on success or an error code on failure.
int create_tables();

#endif // DB_TABLES_H
