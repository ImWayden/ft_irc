#include <iostream>
#include <string>
#include <vector>
#include <iomanip> // pour std::hex, std::setw, std::setfill

static uint32_t hashCommand(const std::string &cmd) {
    if (cmd.size() < 4)
        return 0xFFFFFFFF;
    uint32_t hash = 0;
    for (size_t i = 0; i < 4; ++i) {
        hash <<= 8;
        hash |= static_cast<unsigned char>(cmd[i]);
    }
    return hash;
}

int main() {
    std::vector<std::string> commands = {
        "PASS",
        "NICK",
        "USER",
        "QUIT",
        "PING",
        "PONG",
        "JOIN",
        "PART",
        "PRIVMSG",
        "KICK",
        "INVITE",
        "TOPIC",
        "MODE"
    };

    for (const auto& cmd : commands) {
        uint32_t hash = hashCommand(cmd);
        std::cout << std::setw(7) << std::left << cmd << " -> 0x"
                  << std::hex << std::uppercase << std::setfill(' ') << std::setw(8)
                  << hash << std::dec << std::endl;
    }

    return 0;
}