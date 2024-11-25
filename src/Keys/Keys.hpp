#include "admin_key.h"
#include "aero_dynamics_key.h"
#include "delta_sim_key.h"
#include "group_key.h"
#include "lunar_sim_key.h"
#include "ouroboros_key.h"
#include "qbit_sim_key.h"

#include <cstdint>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

inline const char *BuildKeyFromBinary(const std::vector<uint8_t> &key) {
    char *key_cstr = new char[key.size() + 1];
    std::memcpy(key_cstr, key.data(), key.size());
    key_cstr[key.size()] = '\0';
    return key_cstr;
}

inline std::unordered_map<std::string, std::vector<uint8_t>> GetKeyMap() {
    std::unordered_map<std::string, std::vector<uint8_t>> map;

    std::vector<uint8_t> admin_key_vec(admin_key, admin_key + admin_key_len);
    map.insert({"admin_key", admin_key_vec});

    std::vector<uint8_t> group_key_vec(group_key, group_key + group_key_len);
    map.insert({"group_key", group_key_vec});

    std::vector<uint8_t> aero_dynamics_key_vec(aero_dynamics_key, aero_dynamics_key + aero_dynamics_key_len);
    map.insert({"aero_dynamics_key", aero_dynamics_key_vec});

    std::vector<uint8_t> delta_sim_key_vec(delta_sim_key, delta_sim_key + delta_sim_key_len);
    map.insert({"delta_sim_key", delta_sim_key_vec});

    std::vector<uint8_t> lunar_sim_key_vec(lunar_sim_key, lunar_sim_key + lunar_sim_key_len);
    map.insert({"lunar_sim_key", lunar_sim_key_vec});

    std::vector<uint8_t> ouroboros_key_vec(ouroboros_key, ouroboros_key + ouroboros_key_len);
    map.insert({"ouroboros_key", ouroboros_key_vec});

    std::vector<uint8_t> qbit_sim_key_vec(qbit_sim_key, qbit_sim_key + qbit_sim_key_len);
    map.insert({"qbit_sim_key", qbit_sim_key_vec});

    return map;
}
