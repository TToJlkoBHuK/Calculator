#include "Engine.h"
#include <stdexcept>

void Engine::register_command(ICommand* command, const std::string& commandName) {
    if (command) {
        commands[commandName] = command;
    }
}

int Engine::execute(const std::string& commandName, const std::map<std::string, int>& args) {
    auto it = commands.find(commandName);
    if (it != commands.end()) {
        return it->second->execute(args);
    }
    throw std::runtime_error("Command not found: " + commandName);
}