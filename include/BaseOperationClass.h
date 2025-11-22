#pragma once

#include <vector>

class BaseOperation {
public:
	virtual double operation(const std::vector<double>& arg) const = 0;
	virtual const char* getName() = 0;
};