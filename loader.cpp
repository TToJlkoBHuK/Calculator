#include "loader.h"
#include "Register.h"
#include "PluginAdapter.h"

void loader(Register& registry) {
	WIN32_FIND_DATA FindFile;
	wchar_t current[MAX_PATH];
	GetCurrentDirectoryW(MAX_PATH, current);

	std::wstring PlaginPath = std::wstring(current) + L"/plugins/*.dll";
	HANDLE hFile = FindFirstFile(PlaginPath.c_str(), &FindFile);

	if (hFile == INVALID_HANDLE_VALUE) {
		std::cerr << "plugins not found" << std::endl;
		return;
	}

	do {
		std::wstring Full = std::wstring(current) + L"/plugins/" + std::wstring(FindFile.cFileName);
		HMODULE Module = LoadLibrary(Full.c_str());
		if (Module) {
			FuncPtr func = (FuncPtr)GetProcAddress(Module, "operationE");
			FuncNamePtr name = (FuncNamePtr)GetProcAddress(Module, "getNameE");
			FuncArityPtr arity = (FuncArityPtr)GetProcAddress(Module, "getArityE");

			if (func && name && arity) {
				std::string namefunc = name();
				int arityfunc = arity();

				registry.addOper(std::make_unique<PluginAdapter>(namefunc, func, arityfunc));
				std::cout << "loaded " << namefunc << " arity " << arityfunc << std::endl;
			}
			else {
				std::wcerr << L"not load function from " << Full.c_str() << L" invalid DLL" << std::endl;
				FreeLibrary(Module);
			}
		}
		else {
			std::wcerr << L"not load function " << Full.c_str() << std::endl;
		}

	} while (FindNextFile(hFile, &FindFile) != 0);

	FindClose(hFile);
}