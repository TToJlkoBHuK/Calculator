#include "loader.h"
#include "Register.h"
#include "PluginAdapter.h"

void tryLoadPlugin(const std::wstring& fullPath, Register& registry) {
    HMODULE Module = LoadLibrary(fullPath.c_str());
    if (!Module) {
        std::wcerr << L"not load function " << fullPath.c_str() << std::endl;
        return;
    }

    FuncPtr func = (FuncPtr)GetProcAddress(Module, "operationE");
    FuncNamePtr name = (FuncNamePtr)GetProcAddress(Module, "getNameE");
    FuncArityPtr arity = (FuncArityPtr)GetProcAddress(Module, "getArityE");

    if (func && name && arity) {
        std::string namefunc = name();
        int arityfunc = arity();

        registry.addOper(std::make_unique<PluginAdapter>(namefunc, func, arityfunc, Module));
        std::cout << "loaded " << namefunc << " arity " << arityfunc << std::endl;
    }
    else {
        std::wcerr << L"not load function from " << fullPath.c_str() << L" invalid DLL" << std::endl;
        FreeLibrary(Module);
    }
}

void loader(Register& registry) {
    WIN32_FIND_DATA FindFile;
    wchar_t current[MAX_PATH];
    GetCurrentDirectoryW(MAX_PATH, current);

    std::wstring PluginPath = std::wstring(current) + L"/plugins/*.dll";
    HANDLE hFile = FindFirstFile(PluginPath.c_str(), &FindFile);

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "plugins not found" << std::endl;
        return;
    }

    do {
        std::wstring Full = std::wstring(current) + L"/plugins/" + std::wstring(FindFile.cFileName);
        tryLoadPlugin(Full, registry);

    } while (FindNextFile(hFile, &FindFile) != 0);

    FindClose(hFile);
}