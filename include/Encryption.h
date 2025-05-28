#pragma once

#include <string>

std::string base64_encode(const unsigned char* buffer, size_t length);
std::string hmac_sha256(const std::string& key, const std::string& data);