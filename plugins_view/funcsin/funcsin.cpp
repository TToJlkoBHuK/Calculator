#include "pch.h"
#include "funcsin.h"
#include <memory>

double FuncSin::operation(const std::vector<double>& arg) const {
	if (arg.size() == 1) {
		return sin(arg[0]);
	}
	throw ErrorMessage("operation sin have 1 arg");
}

const char* FuncSin::getName() {
	return "sin";
}

extern "C" __declspec(dllexport) double operationE(const std::vector<double>&arg) {
	std::unique_ptr<BaseOperation> baseoper = std::make_unique<FuncSin>();
	return baseoper->operation(arg);
}

extern "C" __declspec(dllexport) const char* getNameE() {
	std::unique_ptr<BaseOperation> name = std::make_unique<FuncSin>();
	return name->getName();
}

extern "C" __declspec(dllexport) int getArityE() {
	return 1;
}