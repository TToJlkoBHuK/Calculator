#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <map>
#include "vocabulary.h"
#include "Operator.h"
#include "Register.h"



struct Tree {
	Vocab value;
	std::unique_ptr<Tree> left;
	std::unique_ptr<Tree> right;

	IOperator* operation;

	Tree(const Vocab val, IOperator* op = nullptr)
		: value(val), left(nullptr), right(nullptr), operation(op) {}
};

std::unique_ptr<Tree> build(std::vector<Vocab>& vocabs, const Register& registry);

double evaluate(const std::unique_ptr<Tree> top);