#include "pch.h"
#include "funccos.h"
#include <memory>

double FuncCos::operation(const std::vector<double>& arg) const {
	if (arg.size() == 1) {
		return cos(arg[0]);
	}
	throw ErrorMessage("operation cos have 1 arg");
}

const char* FuncCos::getName() {
	return "cos";
}

extern "C" __declspec(dllexport) double operationE(const std::vector<double>&arg) {
	std::unique_ptr<BaseOperation> baseoper = std::make_unique<FuncCos>();
	return baseoper->operation(arg);
}

extern "C" __declspec(dllexport) const char* getNameE() {
	std::unique_ptr<BaseOperation> name = std::make_unique<FuncCos>();
	return name->getName();
}

extern "C" __declspec(dllexport) int getArityE() {
	return 1;
}