#pragma once

#include <string>

namespace MR{
class FileSystem {
    public:
        static std::string GetWorkingDirectory();
        static bool ReadFile(std::string path, std::string& content, bool absolutePath = false);
        static bool ReadImage(std::string path, unsigned char*& data, int& width, int& height, int& nrChannels, bool flipVertically = true, bool absolutePath = false);
};
}
