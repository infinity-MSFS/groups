
#include "Deserialize.hpp"

#include "zlib.h"
#include "msgpack.hpp"

Types::Incoming::IncomingBinary Deserialize::GetIncomingBinary() {
    z_stream strm{};
    strm.next_in = m_RawData.data();
    strm.avail_in = static_cast<uInt>(m_RawData.size());

    if (inflateInit2(&strm, 16 + MAX_WBITS) != Z_OK) {
        throw std::runtime_error("Failed to initialize zlib for decompression");
    }

    std::vector<uint8_t> decompressed_data;
    std::array<uint8_t, 8191> buffer{};

    int ret;
    do {
        strm.next_out = buffer.data();
        strm.avail_out = static_cast<uInt>(buffer.size());

        ret = inflate(&strm, Z_NO_FLUSH);
        if (ret != Z_OK && ret != Z_STREAM_END) {
            inflateEnd(&strm);
            throw std::runtime_error("Decompression error");
        }

        const size_t decompressed_size = buffer.size() - strm.avail_out;
        decompressed_data.insert(decompressed_data.end(), buffer.data(), buffer.data() + decompressed_size);
    } while (ret != Z_STREAM_END);
    inflateEnd(&strm);

    try {
        const msgpack::object_handle object_handle = msgpack::unpack(reinterpret_cast<const char *>(decompressed_data.data()), decompressed_data.size());
        const msgpack::object object = object_handle.get();

        Types::Incoming::IncomingBinary incoming_binary;
        object.convert(incoming_binary);
        return incoming_binary;
    } catch (std::exception &e) {
        throw std::runtime_error(std::string("MessagePack deserialization error: ") + e.what());
    }
}
