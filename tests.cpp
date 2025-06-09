#include <iostream>
#include "sqlite3.h"
#include "db_tables.h"
#include "db_insert.h"
#include "sha256_util.h"    // NEW: for SHA-256 hashing of the master password
#include "aes256.h"
#include "master_password.h"
#include <limits>
#include <cassert>

std::string masterPlain;  // NEW: holds verified master password during runtime
sqlite3* db = nullptr;

// Fixed key for testing
const string masterPlainPassword = "test_key_1234";

void testEncryptionRoundTrip() {

    string plaintext = "TestPassword123!";
    string encrypted = aes256(true, masterPlainPassword, plaintext);
    string decrypted = aes256(false, masterPlainPassword, encrypted);
    assert(decrypted == plaintext);

}

void testEncryptionOutputDiffers() {

    string plaintext = "password";
    string encrypted = aes256(true, masterPlainPassword, plaintext);
    assert(encrypted != plaintext);

}

void testInsertCredential() {

    SQL_vaultWriter("test.com", "user1", "pass1");
    bool found = SQL_vaultSearch("test.com");
    assert(found);

}

int main() {
    
    testEncryptionRoundTrip();
    testEncryptionOutputDiffers();
    testInsertCredential();

    cout << "All tests passed \n";
    
    return 0;
}