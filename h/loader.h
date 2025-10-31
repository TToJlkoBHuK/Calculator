#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <map>

class Register;

typedef double (*FuncPtr)(const std::vector<double>&);
typedef const char* (*FuncNamePtr)();
typedef int (*FuncArityPtr)();

void loader(Register& registry);