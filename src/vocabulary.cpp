#include <string>
#include "vocabulary.h"

Vocab::Vocab(Type type, std::string value) {
	this->type = type;
	this->value = value;
}