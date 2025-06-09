#include "aes256.h"
#include "db_insert.h"
#include "db_tables.h"
#include "master_password.h"
#include "sha256_util.h"
#include <cassert>
#include <iostream>


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

int run_tests() {
    
    testEncryptionRoundTrip();
    testEncryptionOutputDiffers();
    testInsertCredential();

    cout << "All tests pased \n";
    
    return 0;
}