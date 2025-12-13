#pragma once

#include <string>

enum class Type {
	NUM,
	OPR,
	BIN_OPR,
	UNA_OPR,
	BRC
};

struct Vocab {
public:
	Type type;
	std::string value;

	Vocab(Type type, std::string value);
};