
#include "Fetcher/Fetcher.hpp"
#include "Builder/Builder.hpp"
#include "Deserialize/Deserialize.hpp"
#include "Encryption/Encryption.hpp"
#include "BinaryStructure/BinaryStructure.hpp"

#ifdef INFINITY_TESTS
#include <filesystem>
#include <fstream>

std::vector<unsigned char> readBinaryFile(const std::filesystem::path &filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Error opening file: " + filePath.string());
    }

    const std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer(size);
    if (!file.read(reinterpret_cast<char *>(buffer.data()), size)) {
        throw std::runtime_error("Error reading file: " + filePath.string());
    }

    return buffer;
}
#endif


int main(int argc, char *argv[]) {
    std::vector<Types::Incoming::GroupData> active_groups_raw;
#ifdef INFINITY_TESTS

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <Bin Directory>" << std::endl;
        return 1;
    }

    std::filesystem::path directory(argv[1]);

    if (!exists(directory) || !is_directory(directory)) {
        std::cerr << "Invalid directory: " << directory << std::endl;
        return 1;
    }


    try {
        for (const auto &entry: std::filesystem::directory_iterator(directory)) {
            if (is_regular_file(entry) && entry.path().extension() == ".bin") {
                std::vector<unsigned char> binaryContent = readBinaryFile(entry.path());
                auto unencrypted_bin = Encryption::Encryption::DecryptBinary(binaryContent, Encryption::keys::admin_key);
                const auto deserializer = new Deserialize(unencrypted_bin);
                auto [groupData, key] = deserializer->GetIncomingBinary();
                if (const auto decrypted_key = Encryption::Encryption::DecryptFromBase64(key, Encryption::keys::group_decrypt_key);
                    decrypted_key != Encryption::keys::GetKeyFromString(groupData.name)) {
                    throw std::invalid_argument("Wrong key for group " + groupData.name);
                }
                active_groups_raw.emplace_back(groupData);
            }
        }
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }


#else
    std::string active_group_url = "https://github.com/infinity-MSFS/groups/raw/refs/heads/main/activeGroups.cfg";

    const auto groups_raw = Fetcher::FetchString(active_group_url);
    auto active_group_deserializer = new DeserializeActiveGroups(groups_raw, ';');
    auto groups = active_group_deserializer->GetActiveGroups();
    delete active_group_deserializer;


    for (const auto &group: groups) {
        std::string url = "https://github.com/infinity-MSFS/groups/blob/" + group + "/";
        url.append(group + ".bin");
        auto group_bin = Fetcher::FetchBinary(url);
        auto unencrypted_bin = Encryption::Encryption::DecryptBinary(group_bin, Encryption::keys::admin_key);
        const auto deserializer = new Deserialize(unencrypted_bin);
        auto [groupData, key] = deserializer->GetIncomingBinary();
        if (const auto decrypted_key = Encryption::Encryption::DecryptFromBase64(key, Encryption::keys::group_decrypt_key); decrypted_key != Encryption::keys::GetKeyFromString(group)) {
            throw std::invalid_argument("Wrong key for group " + group);
        }
        active_groups_raw.emplace_back(groupData);
    }
#endif

    std::vector<Types::Outgoing::GroupData> outgoing_groups_raw;

    for (const auto &group: active_groups_raw) {
        Types::Outgoing::GroupData new_group_data;
        std::vector<Types::Outgoing::Project> projects;
        for (const auto &project: group.projects) {
            const auto changelog = Fetcher::FetchString(project.changelogLink);
            const auto new_project = Types::Outgoing::Project{
                project.name,
                project.description,
                project.overview,
                changelog,
                project.background,
                project.pageBackground,
                project.variants,
                {project.packages}
            };
            projects.emplace_back(new_project);
        }
        new_group_data.name = group.name;
        new_group_data.projects = projects;
        new_group_data.logo = group.logo;
        new_group_data.betaBackground = group.betaBackground;
        new_group_data.palette = Types::Outgoing::Palette{
            group.palette.primary,
            group.palette.secondary,
            group.palette.circle1,
            group.palette.circle2,
            group.palette.circle3,
            group.palette.circle4,
            group.palette.circle5,
        };
        new_group_data.isHidden = false;

        outgoing_groups_raw.emplace_back(new_group_data);
    }

    std::map<std::string, Types::Outgoing::GroupData> outgoing_groups;
    for (const auto &group: outgoing_groups_raw) {
        outgoing_groups.emplace(group.name, group);
    }

    const auto compressed_output = Builder::SerializeAndCompress(outgoing_groups);
    const auto encrypted_output = Encryption::Encryption::EncryptBinary(compressed_output, Encryption::keys::admin_key);

    Builder::WriteToFile("groups.bin", encrypted_output);

    return 0;
}
