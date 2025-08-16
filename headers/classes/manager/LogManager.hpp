/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogManager.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 08:17:57 by wayden            #+#    #+#             */
/*   Updated: 2025/08/16 19:43:23 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGMANAGER_HPP
#define LOGMANAGER_HPP

#include <string>
#include <fstream>
#include <ctime>
#include <iostream>

enum LogType {
    INFO,
    WARNING,
    ERROR,
    IRC_ACTIVITY,
    SERVER_TECH
};

class LogManager {
public:
	static void onClose();
    static void log(LogType type, const std::string& message);

    static void logInfo(const std::string& message);
    static void logWarning(const std::string& message);
    static void logError(const std::string& message);
    static void logIRCActivity(const std::string& message);
    static void logServerTech(const std::string& message);

private:
    LogManager();
    ~LogManager();

    static LogManager* instance();

    std::ofstream _globalLog;
    std::ofstream _warnErrLog;
    std::ofstream _ircActivityLog;
    std::ofstream _serverTechLog;

    void openLogFiles();
    void rotateLogFile(const std::string& filePath);
    void writeTo(std::ofstream& file, const std::string& type, const std::string& message);
    std::string timestampLine();

    static LogManager* _instance;
};

#endif
