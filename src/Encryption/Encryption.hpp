
#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "openssl/aes.h"
#include "openssl/bio.h"
#include "openssl/buffer.h"
#include "openssl/evp.h"
#include "openssl/rand.h"

#include "Keys/Keys.hpp"

namespace Encryption {
    class Encryption {
    public:
        static std::vector<unsigned char> Encrypt(const std::string &plaintext, const std::string &key);

        static std::vector<uint8_t> EncryptBinary(const std::vector<uint8_t> &binary_input, const std::string &key);

        static std::string Decrypt(const std::vector<unsigned char> &ciphertext, const std::string &key);

        static std::vector<uint8_t> DecryptBinary(const std::vector<uint8_t> &ciphertext, const std::string &key);


        static std::string EncryptToBase64(const std::string &plaintext, const std::string &key);

        static std::string DecryptFromBase64(const std::string &base64_data, const std::string &key);

    private:
        static std::vector<unsigned char> GenerateIV();

        static std::string Base64Encode(const std::vector<unsigned char> &binary_data);

        static std::vector<unsigned char> Base64Decode(const std::string &encoded_data);
    };


    namespace keys {


        inline const char *GetKey(const char *name) {
            auto key_map = GetKeyMap();
            if (const auto it = key_map.find(name); it != key_map.end()) {
                return BuildKeyFromBinary(it->second);
            }
            return "";
        }

        // inline const std::string GetKeyFromString(const std::string &snake_case) {
        //     static const std::unordered_map<std::string, std::string> key_map = {
        //             {"admin", admin_key},         {"group_decrypt", group_decrypt_key}, {"aero_dynamics", aero_dynamics_key}, {"delta_sim", delta_sim_key},
        //             {"lunar_sim", lunar_sim_key}, {"ouroboros", ouroboros_key},         {"qbit_sim", qbit_sim_key},
        //     };

        //     if (const auto it = key_map.find(snake_case); it != key_map.end()) {
        //         return it->second;
        //     }
        //     throw std::runtime_error("Invalid key: " + snake_case);
        // }
    } // namespace keys

    enum class ValidationTypes { FULL, AERO_DYNAMICS, DELTA_SIM, LUNAR_SIM, OUROBOROS, QBIT_SIM, NOT_AUTHORIZED };

    class Validation {
    public:
        explicit Validation(const char *pub_key);

        [[nodiscard]] ValidationTypes GetValidationType();

    private:
        void ParsePublicKey();

    private:
        const char *m_PublicKey;
        std::optional<const char *> m_PrivateKey;
        ValidationTypes m_ValidationType;
    };
} // namespace Encryption
