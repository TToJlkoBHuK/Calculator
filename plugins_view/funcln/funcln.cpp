#include "pch.h"
#include "funcln.h"

double FuncLn::operation(const std::vector<double>& arg) const {
	if (arg.size() == 1) {
		if (arg[0] <= 0) throw ErrorMessage("you mast have a positive arg");
		return log(arg[0]);
	}
	throw ErrorMessage("operation ln have 1 arg");
}

const char* FuncLn::getName() {
	return "ln";
}

extern "C" __declspec(dllexport) double operationE(const std::vector<double>&arg) {
	std::unique_ptr<BaseOperation> baseoper = std::make_unique<FuncLn>();
	return baseoper->operation(arg);
}

extern "C" __declspec(dllexport) const char* getNameE() {
	std::unique_ptr<BaseOperation> name = std::make_unique<FuncLn>();
	return name->getName();
}

extern "C" __declspec(dllexport) int getArityE() {
	return 1;
}