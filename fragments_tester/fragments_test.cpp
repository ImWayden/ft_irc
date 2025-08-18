#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Socket error\n";
        return 1;
    }

    sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(6667);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connect failed\n";
        return 1;
    }

    // Exemple de fragments pour simuler le test du sujet
    const char* fragments[] = {
        "PRI",           // fragment 1
        "VMSG #ch",      // fragment 2
        "annel :Hi\r\n"  // fragment final avec \r\n
    };

    for (size_t i = 0; i < 3; ++i) {
        send(sock, fragments[i], strlen(fragments[i]), 0);
        usleep(20000000); // 0.2s de pause pour simuler fragmentation
    }

    std::cout << "Fragments sent!\n";

    // Optionnel : recevoir la réponse du serveur
    char buffer[1024];
    ssize_t n = read(sock, buffer, sizeof(buffer)-1);
    if (n > 0) {
        buffer[n] = '\0';
        std::cout << "Server response: " << buffer;
    }

    close(sock);
    return 0;
}