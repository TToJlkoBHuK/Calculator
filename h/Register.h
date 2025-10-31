#pragma once
#include <map>
#include <string>
#include <memory>
#include "Operator.h"

class Register {
public:
	Register();
	void addOper(std::unique_ptr<IOperator> op);
	IOperator* getOper(const std::string& name) const;

	void loadPlug();

private:
	std::map < std::string, std::unique_ptr<IOperator>> m_operations;
};