#include <iostream>
#include <stdio.h>

#include "Mantaray/Core/Logger.h"

#ifdef PLATFORM_WINDOWS
#include <windows.h>

#define FOREGROUND_WHITE 0x0007
#define FOREGROUND_YELLOW 0x0006

void changeColor(WORD theColor);

void changeColor(WORD theColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, theColor);
#endif

#ifdef PLATFORM_LINUX
#define FOREGROUND_WHITE "\033[37m"
#define FOREGROUND_YELLOW "\033[33m"
#define FOREGROUND_RED "\033[31m"
#define FOREGROUND_BLUE "\033[34m"

#define changeColor(color) logPrefix = color; message += "\033[0m";
#endif

using namespace MR;

Logger::Logger(std::string name) {
    m_name = name;
}

void Logger::Log(std::string message, Logger::LogLevel logLevel) {
    Logger::Log(m_name, message, logLevel);
}

void Logger::Log(std::string name, std::string message, Logger::LogLevel logLevel) {
    std::string logPrefix = "";
    switch (logLevel)
    {
    case Logger::LOG_INFO:
        changeColor(FOREGROUND_WHITE);
        logPrefix += "INFO::" + name + "-> ";
        break;
    case Logger::LOG_WARNING:
        changeColor(FOREGROUND_YELLOW);
        logPrefix += "WARNING::" + name + "-> ";
        break;
    case Logger::LOG_ERROR:
        changeColor(FOREGROUND_RED);
        logPrefix += "ERROR::" + name + "-> ";
        break;
    case Logger::LOG_DEBUG:
        changeColor(FOREGROUND_BLUE);
        logPrefix += "DEBUG::" + name + "-> ";
        break;
    
    default:
        return;
    }

    std::cout << logPrefix + message << std::endl;
    changeColor(FOREGROUND_WHITE);
}
