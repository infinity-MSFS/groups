#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <memory>

#include "curl/curl.h"
#include "zlib.h"
#include "Json/json.hpp"
#include "msgpack.hpp"

struct Project
{
    std::string name;
    std::string refLink;
    std::string changelogLink;
    std::string background;
    std::string pageBackground;
    std::vector<std::string> variants;
};

struct Package
{
    bool isGithub;
    std::string link;
};

struct Palette
{
    std::string primary;
    std::string secondary;
    std::string circle1;
    std::string circle2;
    std::string circle3;
    std::string circle4;
    std::string circle5;
};

struct GroupData
{
    std::string name;
    std::vector<Project> projects;
    std::string logo;
    std::string betaBackground;
    Palette palette;
    bool isHidden;
    unsigned int key;
};

std::map<std::string, GroupData> decode_bin(const std::vector<uint8_t> &raw_data)
{
    // Step 1: Decompress the data
    z_stream strm{};
    strm.next_in = reinterpret_cast<Bytef *>(const_cast<uint8_t *>(raw_data.data()));
    strm.avail_in = static_cast<uInt>(raw_data.size());

    if (inflateInit2(&strm, 16 + MAX_WBITS) != Z_OK)
    {
        throw std::runtime_error("Failed to initialize zlib for decompression");
    }

    std::vector<uint8_t> decompressed_data;
    std::array<uint8_t, 8192> buffer;

    int ret;
    do
    {
        strm.next_out = buffer.data();
        strm.avail_out = static_cast<uInt>(buffer.size());

        ret = inflate(&strm, Z_NO_FLUSH);
        if (ret != Z_OK && ret != Z_STREAM_END)
        {
            inflateEnd(&strm);
            throw std::runtime_error("Decompression error");
        }

        size_t decompressed_size = buffer.size() - strm.avail_out;
        decompressed_data.insert(decompressed_data.end(), buffer.data(), buffer.data() + decompressed_size);
    } while (ret != Z_STREAM_END);

    inflateEnd(&strm);

    // Step 2: Deserialize the decompressed data (MessagePack)
    try
    {
        msgpack::object_handle oh = msgpack::unpack(reinterpret_cast<const char *>(decompressed_data.data()), decompressed_data.size());
        msgpack::object obj = oh.get();

        std::map<std::string, GroupData> group_data;
        obj.convert(group_data);
        return group_data;
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("MessagePack deserialization error: ") + e.what());
    }
}
