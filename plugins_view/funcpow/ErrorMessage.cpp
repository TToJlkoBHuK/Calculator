#include "ErrorMessage.h"

ErrorMessage::ErrorMessage(std::string message) : message{ message } {};

std::string ErrorMessage::getMsg() const {
	return message;
}