
#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <memory>

#include "curl/curl.h"

inline size_t WriteToBufferCallback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    if (!userdata)
        return 0;

    auto *buffer = static_cast<std::vector<uint8_t> *>(userdata);
    const size_t bytes_to_write = size * nmemb;

    try {
        const size_t current_size = buffer->size();
        buffer->resize(current_size + bytes_to_write);
        std::copy_n(ptr, bytes_to_write, buffer->data() + current_size);
        return bytes_to_write;
    } catch (const std::exception &) {
        return 0;
    }
}

inline size_t WriteStringCallback(void *contents, size_t size, size_t nmemb, std::string *userData) {
    const size_t totalSize = size * nmemb;
    userData->append(static_cast<char *>(contents), totalSize);
    return totalSize;
}

class CurlGuard {
public:
    explicit CurlGuard(CURL *curl) : m_Curl(curl) {
    }

    ~CurlGuard() { if (m_Curl) { curl_easy_cleanup(m_Curl); } }

    CurlGuard(const CurlGuard &) = delete;

    CurlGuard &operator=(const CurlGuard &) = delete;

private:
    CURL *m_Curl;
};

class Fetcher {
public:
    static std::vector<uint8_t> FetchBinary(const std::string &url);

    static std::string FetchString(const std::string &url);
};
