
#pragma once

#include <string>
#include <vector>
#include <map>

#include "BinaryStructure/BinaryStructure.hpp"

class Builder {
public:
    static std::vector<uint8_t> SerializeAndCompress(const std::map<std::string, Types::Outgoing::GroupData> &data);

    static void WriteToFile(const std::string &filename, const std::vector<uint8_t> &data);
};
