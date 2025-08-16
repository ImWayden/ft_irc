/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 13:27:16 by wayden            #+#    #+#             */
/*   Updated: 2025/08/16 19:31:16 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manager/ServerManager.hpp"
#include <cstring>          // memset()
#include <csignal>

volatile sig_atomic_t g_running = 1;

void handle_sigint(int sig) {
	if (sig == SIGINT)
		LogManager::logWarning("SIGINT received, serveur brutally stopped");
	else if (sig == SIGTERM)
		LogManager::logWarning("SIGTERM received, serveur killed gracefully");
    g_running = 0; // demande d’arrêt
}

int main(int argc, char **argv)
{
	if(argc != R_ARGC)
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	std::string port = argv[1];
	std::string password = argv[2];

	ServerManager server(port, password);
	int status = server.Init();
	std::signal(SIGINT, handle_sigint);
	std::signal(SIGTERM, handle_sigint);
	if(status){
		LogManager::logError("Failed to initialize server");
		LogManager::onClose();
		std::cerr << "Failed to initialize server check the logs for more informations" << std::endl;
		return 1;
	}
	while(g_running && !status)
	{
		server.Update();
		server.onUpdateFinish();
	}
	server.OnClose();
	LogManager::onClose();
	return 0;
}

