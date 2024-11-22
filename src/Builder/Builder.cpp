#include <fstream>
#include <stdexcept>

#include "Builder.hpp"
#include "msgpack.hpp"
#include "zlib.h"

std::vector<uint8_t> Builder::SerializeAndCompress(const std::map<std::string, Types::Outgoing::GroupData> &data) {
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, data);

    z_stream strm{};
    if (deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 16 + MAX_WBITS, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        throw std::runtime_error("Failed to initialize zlib for compression");
    }

    std::vector<uint8_t> compressed_data;
    std::array<uint8_t, 8192> buffer{};

    strm.next_in = reinterpret_cast<Bytef *>(sbuf.data());
    strm.avail_in = static_cast<uInt>(sbuf.size());

    int ret;
    do {
        strm.next_out = buffer.data();
        strm.avail_out = static_cast<uInt>(buffer.size());

        ret = deflate(&strm, Z_FINISH);
        if (ret != Z_OK && ret != Z_STREAM_END) {
            deflateEnd(&strm);
            throw std::runtime_error("Compression error");
        }

        const size_t compressed_size = buffer.size() - strm.avail_out;
        compressed_data.insert(compressed_data.end(), buffer.data(), buffer.data() + compressed_size);
    } while (ret != Z_STREAM_END);

    deflateEnd(&strm);
    return compressed_data;
}

void Builder::WriteToFile(const std::string &filename, const std::vector<uint8_t> &data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for writing");
    }
    file.write(reinterpret_cast<const char *>(data.data()), static_cast<int>(data.size()));
    if (!file) {
        throw std::runtime_error("Failed to write data to file");
    }
    file.close();
}
