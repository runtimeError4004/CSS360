#include "aes256.h"
#include "db_insert.h"
#include "db_tables.h"
#include "master_password.h"
#include "sha256_util.h"
#include <cassert>
#include <iostream>


// Fixed key for testing
const string masterPlainPassword = "test_key_1234";

// Check that encryption -> decryption returns the original text
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

void testSearchCredential() {

    SQL_vaultWriter("search.com", "searchUser", "searchPass");
    bool found = SQL_vaultSearch("search.com");
    assert(found);

}

void testSearchNonExistingCredential() {

    string searchKey = "fake.com";
    bool found = SQL_vaultSearch(searchKey);
    assert(!found);

}

void testDeleteCredential() {

    SQL_vaultWriter("delete.com", "deleteUser", "deletePass");
    assert(SQL_vaultSearch("delete.com"));
    SQL_vaultSearchDelete("delete.com");
    bool found = SQL_vaultSearch("delete.com");
    assert(!found);

}

int run_tests() {
    
    testEncryptionRoundTrip();
    testEncryptionOutputDiffers();
    testInsertCredential();
    testSearchCredential();
    testDeleteCredential();
    testSearchNonExistingCredential();

    cout << "All tests pased \n";
    
    return 0;
}