#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "Deserialize.hpp"

class Builder
{
public:
    Builder()
    {
    }
    void FetchBin()
    {
    }

private:
    std::vector<unsigned int> m_RawData;
    std::map<std::string, GroupData> m_Groups;
};

int main()
{
    // deserialize bins

    // decrypt key, if key != master key + encrypted return;

    // else create std::map<std::string,GroupData>

    // map -> JSON -> msgpack -> encryptiopn -> gz -> push

    std::string ouroboros = "@ouroboros/ouroboros.bin";
    std::cout << "Hello, World!" << std::endl;
    return 0;
}