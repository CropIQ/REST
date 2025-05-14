#pragma once

#include <iostream>
#include <string>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

namespace std {
    std::string base64_encode(const unsigned char* buffer, size_t length) {
        BIO* bio, *b64;
        BUF_MEM* bufferPtr;

        b64 = BIO_new(BIO_f_base64());
        // No newlines
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
        bio = BIO_new(BIO_s_mem());
        bio = BIO_push(b64, bio);

        BIO_write(bio, buffer, static_cast<int>(length));
        BIO_flush(bio);
        BIO_get_mem_ptr(bio, &bufferPtr);

        std::string result(bufferPtr->data, bufferPtr->length);
        BIO_free_all(bio);
        return result;
    }

    std::string hmac_sha256(const std::string& key, const std::string& data) {
        unsigned int len = SHA256_DIGEST_LENGTH;
        unsigned char hash[SHA256_DIGEST_LENGTH];

        HMAC(EVP_sha256(),
            key.c_str(), (int) key.length(),
            reinterpret_cast<const unsigned char*>(data.c_str()), (int) data.length(),
            hash, &len);

        return base64_encode(hash, len);
    }
}