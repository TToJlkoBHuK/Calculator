#pragma once
#include "Operator.h"
#include <stdexcept>

class Unary : public IOperator {
public:
	int getAr() const override { return 1; }
};

class Binary : public IOperator {
public:
	int getAr() const override { return 2; }
};

class Add : public Binary {
public:
	std::string getName() const override { return "+"; }
	double execute(const std::vector<double>& args) const override {
		if (args.size() != 2) throw std::runtime_error("+ have 2 arguments");
		return args[0] + args[1];
	}
};

class Sub : public Binary {
public:
	std::string getName() const override { return "-"; }
	double execute(const std::vector<double>& args) const override {
		if (args.size() != 2) throw std::runtime_error("- have 2 arguments");
		return args[0] - args[1];
	}
};

class Multy : public Binary {
public:
	std::string getName() const override { return "*"; }
	double execute(const std::vector<double>& args) const override {
		if (args.size() != 2) throw std::runtime_error("* have 2 arguments");
		return args[0] * args[1];
	}
};

class Div : public Binary {
public:
	std::string getName() const override { return "/"; }
	double execute(const std::vector<double>& args) const override {
		if (args.size() != 2) throw std::runtime_error("/ have 2 arguments");
		if (args[1] == 0) throw std::runtime_error("Division by zero");
		return args[0] / args[1];
	}
};

class Neg : public Unary {
public:
	std::string getName() const override { return "neg"; }
	double execute(const std::vector<double>& args) const override {
		if (args.size() != 1) throw std::runtime_error("unar - have 1 argument");
		return -args[0];
	}
};