#pragma once
#include <string>

// When encrypt==true, encrypt `input` with a key derived from `password`
// and return a hex-encoded blob: IV||ciphertext.
// When encrypt==false, hex-decode `input`, split IV/ciphertext, decrypt with key(password),
// and return the plaintext.
std::string aes256(bool encrypt,
                   const std::string& password,
                   const std::string& input);
