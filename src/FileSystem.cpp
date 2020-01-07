#include <fstream>
#include <streambuf>

#include <Windows.h>

#include "Mantaray/Core/FileSystem.h"
#include "Mantaray/Core/Logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace MR;

std::string FileSystem::getWorkingDirectory() {
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
}

bool FileSystem::readFile(std::string path, std::string& content, bool absolutePath) {
    if (!absolutePath){
        path = FileSystem::getWorkingDirectory() + path;
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

bool FileSystem::loadImage(std::string path, unsigned char*& data, int& width, int& height, int& nrChannels, bool absolutePath) {
    if (!absolutePath) {
        path = FileSystem::getWorkingDirectory() + path;
    }

    data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        Logger::Log("FileSystem", "Image from " + path + " could not be loaded", MR::Logger::LOG_ERROR);
        return false;
    }
    return true;
}
