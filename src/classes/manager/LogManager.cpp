/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogManager.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 08:19:18 by wayden            #+#    #+#             */
/*   Updated: 2025/08/16 19:43:56 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manager/LogManager.hpp"
#include <sstream>
#include <cstdio>
#include <sys/stat.h>
#include <stdio.h>

LogManager* LogManager::_instance = NULL;


LogManager* LogManager::instance() {
    if (!_instance) {
        _instance = new LogManager();
    }
    return _instance;
}

LogManager::LogManager() {
    struct stat info;
    const char* dir = "./logs";
    if (stat(dir, &info) != 0) {
        mkdir(dir, 0755);
    }
    openLogFiles();
}

LogManager::~LogManager() {
    if (_globalLog.is_open()) _globalLog.close();
    if (_warnErrLog.is_open()) _warnErrLog.close();
    if (_ircActivityLog.is_open()) _ircActivityLog.close();
    if (_serverTechLog.is_open()) _serverTechLog.close();
}

void LogManager::rotateLogFile(const std::string& filePath) {
    std::string oldPath = filePath.substr(0, filePath.find_last_of('.')) + "_old.log";

    // Supprime old si existe
    if (FILE* f = fopen(oldPath.c_str(), "r")) {
        fclose(f);
        remove(oldPath.c_str());
    }
    // Renomme si existant
    if (FILE* f = fopen(filePath.c_str(), "r")) {
        fclose(f);
        rename(filePath.c_str(), oldPath.c_str());
    }
}

void LogManager::openLogFiles() {
    std::string basePath = "./logs/";
    std::string globalPath = basePath + "global.log";
    std::string warnErrPath = basePath + "warnings_errors.log";
    std::string ircPath = basePath + "irc_activity.log";
    std::string techPath = basePath + "server_tech.log";

    rotateLogFile(globalPath);
    rotateLogFile(warnErrPath);
    rotateLogFile(ircPath);
    rotateLogFile(techPath);

    _globalLog.open(globalPath.c_str(), std::ios::out | std::ios::trunc);
    _warnErrLog.open(warnErrPath.c_str(), std::ios::out | std::ios::trunc);
    _ircActivityLog.open(ircPath.c_str(), std::ios::out | std::ios::trunc);
    _serverTechLog.open(techPath.c_str(), std::ios::out | std::ios::trunc);
}

std::string LogManager::timestampLine() {
    time_t now = time(NULL);
    struct tm* tmNow = localtime(&now);
    char buffer[20];
    // Format YYYY-MM-DD_HH-MM-SS
    strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", tmNow);
    return std::string(buffer);
}

void LogManager::writeTo(std::ofstream& file, const std::string& type, const std::string& message) {
    if (file.is_open()) {
        file << "[" << timestampLine() << "][" << type << "] " << message << std::endl;
    }
}

void LogManager::log(LogType type, const std::string& message) {
    LogManager* inst = instance();
    switch(type) {
        case INFO:
            inst->writeTo(inst->_globalLog, "INFO", message);
            break;
        case WARNING:
            inst->writeTo(inst->_globalLog, "WARNING", message);
            inst->writeTo(inst->_warnErrLog, "WARNING", message);
            break;
        case ERROR:
            inst->writeTo(inst->_globalLog, "ERROR", message);
            inst->writeTo(inst->_warnErrLog, "ERROR", message);
            break;
        case IRC_ACTIVITY:
            inst->writeTo(inst->_globalLog, "IRC", message);
            inst->writeTo(inst->_ircActivityLog, "IRC", message);
            break;
        case SERVER_TECH:
            inst->writeTo(inst->_globalLog, "TECH", message);
            inst->writeTo(inst->_serverTechLog, "TECH", message);
            break;
    }
}

void LogManager::onClose() {
	LogManager* inst = instance();
	delete inst;
}

void LogManager::logInfo(const std::string& message) {
    log(INFO, message);
}
void LogManager::logWarning(const std::string& message) {
    log(WARNING, message);
}
void LogManager::logError(const std::string& message) {
    log(ERROR, message);
}
void LogManager::logIRCActivity(const std::string& message) {
    log(IRC_ACTIVITY, message);
}
void LogManager::logServerTech(const std::string& message) {
    log(SERVER_TECH, message);
}
