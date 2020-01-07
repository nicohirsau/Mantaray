#include <iostream>
#include <stdio.h>
#include <windows.h>

#define FOREGROUND_WHITE 0x0007
#define FOREGROUND_YELLOW 0x0006

#include "Mantaray/Core/Logger.h"

using namespace MR;

void changeColor(WORD theColor);

void changeColor(WORD theColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, theColor);
}

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
        logPrefix = "INFO::" + name + "-> ";
        break;
    case Logger::LOG_WARNING:
        changeColor(FOREGROUND_YELLOW);
        logPrefix = "WARNING::" + name + "-> ";
        break;
    case Logger::LOG_ERROR:
        changeColor(FOREGROUND_RED);
        logPrefix = "ERROR::" + name + "-> ";
        break;
    
    default:
        return;
    }

    std::cout << logPrefix + message << std::endl;
}
