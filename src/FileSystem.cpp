#include <fstream>
#include <streambuf>

#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#endif
#ifdef PLATFORM_LINUX
#include <unistd.h>
#endif

#include "Mantaray/Core/FileSystem.hpp"
#include "Mantaray/Core/Logger.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace MR;

std::string FileSystem::GetWorkingDirectory() {
#ifdef PLATFORM_WINDOWS
    // Windows specific way of checking if file exists
    char result[MAX_PATH];
    GetModuleFileName(NULL, result, MAX_PATH);
    std::string wd = "";
    for (int i = 0; i < MAX_PATH; i++){
        wd += result[i];
        if (result[i] == '\0'){
            break;
        }
    }
    return wd.substr(0, wd.find_last_of("\\/") + 1);
#endif
#ifdef PLATFORM_LINUX
    char result[512];
    readlink("/proc/self/exe", result, 512);
    std::string wd(result);
    return wd.substr(0, wd.find_last_of("\\/") + 1);
#endif
}

bool FileSystem::ReadFile(std::string path, std::string& content, bool absolutePath) {
    if (!absolutePath){
        path = FileSystem::GetWorkingDirectory() + path;
    }

    std::ifstream t(path.c_str());
    if (!t){
        Logger::Log("FileSystem", "Could not open file: " + path, Logger::LOG_ERROR);
        return false;
    }
    content.clear();
    content = std::string((std::istreambuf_iterator<char>(t)),
                            std::istreambuf_iterator<char>());
    return true;
}

bool FileSystem::ReadImage(std::string path, unsigned char*& data, int& width, int& height, int& nrChannels, bool absolutePath) {
    if (!absolutePath) {
        path = FileSystem::GetWorkingDirectory() + path;
    }

    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        Logger::Log("FileSystem", "Image from " + path + " could not be loaded", MR::Logger::LOG_ERROR);
        return false;
    }
    return true;
}
