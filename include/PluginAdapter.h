#pragma once
#include "Operator.h"
#include "loader.h"
#include <Windows.h>

class PluginAdapter : public IOperator {
public:
    PluginAdapter(std::string name, FuncPtr func, int arity, HMODULE module)
        : m_name(std::move(name)), m_func(func), m_arity(arity), m_module(module) {
    }
    ~PluginAdapter() override {
        if (m_module) {
            FreeLibrary(m_module);
        }
    }

    double execute(const std::vector<double>& args) const override {
        if (args.size() != m_arity) {
            throw std::runtime_error("Arg != for function " + m_name);
        }
        return m_func(args);
    }

    std::string getName() const override {
        return m_name;
    }

    int getAr() const override {
        return m_arity;
    }

private:
    std::string m_name;
    FuncPtr m_func;
    int m_arity;
    HMODULE m_module;
};