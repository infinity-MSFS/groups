
#pragma once

#include <utility>
#include <vector>
#include <string>
#include <sstream>

#include "BinaryStructure/BinaryStructure.hpp"

class Deserialize {
public:
    explicit Deserialize(const std::vector<uint8_t> &raw_data): m_RawData(raw_data) {
    }

    Types::Incoming::IncomingBinary GetIncomingBinary();

private:
    std::vector<unsigned char> m_RawData;
};

class DeserializeActiveGroups {
public:
    explicit DeserializeActiveGroups(const std::string &raw_data, const char delimiter): m_RawData(raw_data) {
        ParseActiveGroups(delimiter);
    }

    std::vector<std::string> GetActiveGroups() { return m_ActiveGroups; }

private:
    void ParseActiveGroups(const char delimiter) {
        std::stringstream stringStream(m_RawData);
        std::string item;

        while (std::getline(stringStream, item, delimiter)) {
            m_ActiveGroups.emplace_back(std::move(item));
        }
    }

private:
    std::string m_RawData;
    std::vector<std::string> m_ActiveGroups;
};
