#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <stack>
#include "vocabulary.h"
#include "Tree.h"
#include "Register.h"

int Priority(const Vocab& voc, const Register& registry);
std::vector<Vocab> parse(const std::string& s, const Register& registry);
std::vector<Vocab> Postfix(const std::vector<Vocab>& parsed, const Register& registry);

