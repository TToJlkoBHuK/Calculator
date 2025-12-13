#pragma once
#include <vector>
#include <string>
#include <stdexcept>

class IOperator {
public:
	virtual ~IOperator() = default;
	virtual double execute(const std::vector<double>& args) const = 0;
	virtual std::string getName() const = 0;
	virtual int getAr() const = 0;
};