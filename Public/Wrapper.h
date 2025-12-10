#pragma once
#include "ICommand.h"
#include <vector>
#include <string>
#include <functional>
#include <stdexcept>
#include <iostream>

template<typename Object, typename Ret, typename... Args>
class Wrapper : public ICommand {
private:
    Object* obj;                           
    Ret(Object::* method)(Args...);        
    std::vector<std::string> argNames;     

    template<std::size_t... Is>
    Ret call_func(const std::map<std::string, int>& args, std::index_sequence<Is...>) {
        for (const auto& name : argNames) {
            if (args.find(name) == args.end()) {
                throw std::out_of_range("Missing argument: " + name);
            }
        }
        // call method unpacking args
        return (obj->*method)(args.at(argNames[Is])...);
    }

public:
    // C-tor accepts { "name_args": plug }
    Wrapper(Object* obj, Ret(Object::* method)(Args...), const std::map<std::string, int>& argsDef)
        : obj(obj), method(method) {

        //in our task map iterate correct for these keys
        for (const auto& [key, val] : argsDef) {
            argNames.push_back(key);
        }
    }

    int execute(const std::map<std::string, int>& args) override {
        //check arity (quantity args)
        if (args.size() != sizeof...(Args)) {
            throw std::runtime_error("Arguments count mismatch. Expected: " + std::to_string(sizeof...(Args)));
        }

        //run unpacking
        return call_func(args, std::make_index_sequence<sizeof...(Args)>{});
    }
};