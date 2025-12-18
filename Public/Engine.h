#pragma once
#include "ICommand.h"
#include <map>
#include <string>

class Engine {
private:
    std::map<std::string, ICommand*> commands;

public:
    void register_command(ICommand* command, const std::string& commandName);
    int execute(const std::string& commandName, const std::map<std::string, int>& args);
};