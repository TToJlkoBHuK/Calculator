#include "Register.h"
#include "Base_operation.h"
#include "loader.h"
#include <iostream>

Register::Register() {
	addOper(std::make_unique<Add>());
	addOper(std::make_unique<Sub>());
	addOper(std::make_unique<Div>());
	addOper(std::make_unique<Multy>());
	addOper(std::make_unique<Neg>());
}

void Register::addOper(std::unique_ptr<IOperator> op) {
	std::string name = op->getName();
	if (m_operations.count(name)) {
		std::cout << "WARNING: operation " << name << " from a plugin is overriding a built-in operation" << std::endl;

	}
	m_operations[name] = std::move(op);
}

IOperator* Register::getOper(const std::string& name) const {
	auto it = m_operations.find(name);
	if (it != m_operations.end()) {
		return it->second.get();
	}
	return nullptr;
}

void Register::loadPlug() {
	try {
		loader(*this);
	}
	catch (const std::exception& e) {
		std::cerr << "CRITICAL ERROR plugin:" << e.what() << std::endl;

	}

}
