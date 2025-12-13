#pragma once
#define _USE_MATH_DEFINES
#include <Windows.h>
#include <memory>
#include <math.h>

#include "BaseOperationClass.h"
#include "ErrorMessage.h"

class FuncSin : public BaseOperation {
public:
	double operation(const std::vector<double>& arg) const override;

	const char* getName() override;
};

extern "C" __declspec(dllexport) double operationE(const std::vector<double>&arg);
extern "C" __declspec(dllexport) const char* getNameE();
extern "C" __declspec(dllexport) int getArityE();