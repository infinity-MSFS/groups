
#include "Fetcher.hpp"

std::vector<uint8_t> Fetcher::FetchBinary(const std::string &url) {
    std::vector<uint8_t> data;
    CURL *curl = nullptr;
    if (!curl) {
        throw std::runtime_error("Fetcher: curl initialization failed");
    }

    CurlGuard curl_guard(curl);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteToBufferCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Infinity-MSFS-Builder/1.0");


    if (const CURLcode res = curl_easy_perform(curl); res != CURLE_OK) {
        throw std::runtime_error(std::string("Failed to fetch data: ") +
                                 curl_easy_strerror(res));
    }

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    if (http_code != 200) {
        throw std::runtime_error("HTTP error: " + std::to_string(http_code));
    }
    return data;
}

std::string Fetcher::FetchString(const std::string &url) {
    std::string data;
    CURL *curl = nullptr;
    if (!curl) {
        throw std::runtime_error("Fetcher: curl initialization failed");
    }

    CurlGuard curl_guard(curl);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteStringCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Infinity-MSFS-Builder/1.0");


    if (const CURLcode res = curl_easy_perform(curl); res != CURLE_OK) {
        throw std::runtime_error(std::string("Failed to fetch data: ") +
                                 curl_easy_strerror(res));
    }

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    if (http_code != 200) {
        throw std::runtime_error("HTTP error: " + std::to_string(http_code));
    }
    return data;
}

