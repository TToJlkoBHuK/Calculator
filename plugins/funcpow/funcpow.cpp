#include "pch.h"
#include "funcpow.h"
#include <memory>

double FuncPow::operation(const std::vector<double>& arg) const {
	if (arg.size() == 2) {
		return pow(arg[0], arg[1]);
	}
	throw ErrorMessage("operation ^ have 2 arg");
}

const char* FuncPow::getName() {
	return "^";
}

extern "C" __declspec(dllexport) double operationE(const std::vector<double>&arg) {
	std::unique_ptr<BaseOperation> baseoper = std::make_unique<FuncPow>();
	return baseoper->operation(arg);
}

extern "C" __declspec(dllexport) const char* getNameE() {
	std::unique_ptr<BaseOperation> name = std::make_unique<FuncPow>();
	return name->getName();
}

extern "C" __declspec(dllexport) int getArityE() {
	return 2;
}
