#include "Deserialize.hpp"
#include "Fetcher/Fetcher.hpp"
#include "Builder/Builder.hpp"
#include "Deserialize/Deserialize.hpp"
#include "Encryption/Encryption.hpp"
#include "BinaryStructure/BinaryStructure.hpp"


int main(int argc, char **argv) {
    std::string active_group_url = "https://github.com/infinity-MSFS/groups/raw/refs/heads/main/activeGroups.cfg";

    const auto groups_raw = Fetcher::FetchString(active_group_url);
    auto active_group_deserializer = new DeserializeActiveGroups(groups_raw, ';');
    auto groups = active_group_deserializer->GetActiveGroups();
    delete active_group_deserializer;

    std::vector<Types::Incoming::GroupData> active_groups_raw;
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
