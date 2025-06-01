// sha256_util.h
#pragma once
#include <string>

// Computes SHA-256(input) and returns it as a 64-character hex string.
std::string sha256hex(const std::string& input);
