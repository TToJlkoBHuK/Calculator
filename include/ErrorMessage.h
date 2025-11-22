#pragma once
#include <string>

class ErrorMessage : public std::exception {
public:
	ErrorMessage(std::string msg);
	std::string getMsg() const;
private:
	std::string message;
};