#pragma once
#include <map>
#include <string>

//interface command
//abstract clss ICommand
class ICommand {
public:
    virtual int execute(const std::map<std::string, int>& args) = 0;
    virtual ~ICommand() = default;
};