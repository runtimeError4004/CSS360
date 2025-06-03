#include "aes256.h"
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/sha.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>

using namespace CryptoPP;

// Derive a 256-bit key via SHA-256(password), and fill `iv` with either a random
// 128-bit value (when encrypt==true) or the first 16 bytes of SHA-256(key) (when encrypt==false).
static void deriveKeyAndIV(const std::string& password,
                           SecByteBlock& key,
                           SecByteBlock& iv,
                           bool random_iv)
{
    // 1) Compute Key = SHA256(password)
    key = SecByteBlock(AES::MAX_KEYLENGTH);
    SHA256().CalculateDigest(key, (const byte*)password.data(), password.size());

    // 2) Prepare iv container (16 bytes)
    iv = SecByteBlock(AES::BLOCKSIZE);

    if (random_iv) {
        // If encrypting, generate a random 16-byte IV
        AutoSeededRandomPool rnd;
        rnd.GenerateBlock(iv, iv.size());
    }
    else {
        // If decrypting, derive the IV from the SHA256(key) digest:
        //
        //    - Allocate a 32-byte buffer so SHA256() can write its full 32-byte digest
        //    - Then copy only the first 16 bytes into our iv
        byte buf[SHA256::DIGESTSIZE];
        SHA256().CalculateDigest(buf, key, key.size());
        iv.Assign(buf, AES::BLOCKSIZE);
    }
}

std::string aes256(bool encrypt,
                   const std::string& password,
                   const std::string& input)
{
    SecByteBlock key, iv;
    deriveKeyAndIV(password, key, iv, encrypt);

    if (encrypt) {
        // Encrypt plaintext → raw cipher
        std::string cipher;
        CBC_Mode<AES>::Encryption enc;
        enc.SetKeyWithIV(key, key.size(), iv);

        StringSource ss1(
            input,
            true,
            new StreamTransformationFilter(
                enc,
                new StringSink(cipher),
                StreamTransformationFilter::PKCS_PADDING
            )
        );

        // Prepend IV (unencrypted) and hex-encode the result
        std::string iv_and_cipher((char*)iv.data(), iv.size());
        std::string raw = iv_and_cipher + cipher;
        std::string hex;
        StringSource ss2(
            raw,
            true,
            new HexEncoder(new StringSink(hex))
        );
        return hex;
    }
    else {
        // Hex-decode
        std::string decoded;
        StringSource ss3(
            input,
            true,
            new HexDecoder(new StringSink(decoded))
        );
        // Split IV + ciphertext
        std::string iv_in(decoded.data(), AES::BLOCKSIZE);
        std::string cipher_in(decoded.data() + AES::BLOCKSIZE,
                              decoded.size() - AES::BLOCKSIZE);

        // Decrypt
        std::string plaintext;
        CBC_Mode<AES>::Decryption dec;
        dec.SetKeyWithIV(key, key.size(),
                         (const byte*)iv_in.data(), iv_in.size());
        StringSource ss4(
            cipher_in,
            true,
            new StreamTransformationFilter(
                dec,
                new StringSink(plaintext),
                StreamTransformationFilter::PKCS_PADDING
            )
        );
        return plaintext;
    }
}
