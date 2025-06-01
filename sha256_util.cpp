// sha256_util.cpp
#include "sha256_util.h"
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>

using namespace CryptoPP;

// Compute SHA-256 digest of `input`, then hex-encode it.
std::string sha256hex(const std::string& input) {
    byte digest[SHA256::DIGESTSIZE];
    SHA256().CalculateDigest(digest, (const byte*)input.data(), input.size());

    std::string output;
    // HexEncoder(false) produces lowercase hex; you can pass `true` for uppercase.
    StringSource(digest, SHA256::DIGESTSIZE, true,
        new HexEncoder(new StringSink(output), false)
    );
    return output;  // 64-character hex string
}
