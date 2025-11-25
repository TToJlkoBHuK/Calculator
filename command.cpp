#include <iostream>
#include <vector>
#include <map>
#include <exception>
#include <string>
#include <functional>
#include <memory>

// realization abstract interface command
class ICommand {
public:
	virtual int execute(const std::map<std::string, int>& args) = 0;
	virtual ~ICommand() = default;
};

// realization template Wrapper
template<typename Object, typename Ret, typename... Args>
class Wrapper : public ICommand {
private:
	Object* obj;
	Ret(Object::*method)(Args...);
	std::vector<std::string> argNames;
	template<std::size_t... Is>
	Ret call_func(const std::map<std::string, int>& args, std::index_sequence<Is ...>) {
		return (obj->*method)(args.at(argNames[Is])...);
	};
public:
	//c-tor wrapper
	Wrapper(Object* obj, Ret(Object::* method)(Args...), const std::map<std::string, int>& argsDef) : obj(obj), method(method) {
		for (const auto& [key, val] : argsDef) {
			argNames.push_back(key);
		}
	}

	//realization interface ICommand
	int execute(const std::map<std::string, int>& args) override {
		if (args.size() != sizeof... (Args)) {
			throw std::runtime_error("Arguments count mismatch");
		}

		return call_func(args, std::make_index_sequence<sizeof... (Args)>{});
	}
};

//Engine is invoker
class Engine {
private:
	std::map<std::string, ICommand*> commands;
public:
	void register_command(ICommand* command, const std::string& commandName) {
		commands[commandName] = command;
	}

	int execute(const std::string& commandName,const std::map<std::string, int>& args) {
		if (commands.find(commandName) != commands.end()) {
			return commands[commandName]->execute(args);
		}
		throw std::runtime_error("Command not found");
	}
};

//Subject reciver
class Subject {
public:
	int f3(int a, int b) {
		return a + b;
	}
};

//unit test from presentation
//int main() {
//	Subject subj;
//
//	Wrapper wrapper(&subj, &Subject::f3, { {"arg1", 0}, {"arg2", 0} });
//
//	Engine engine;
//
//	engine.register_command(&wrapper, "command1");
//
//	std::cout << engine.execute("command1", { {"arg1", 4}, {"arg2", 5} }) << std::endl;
//
//	return 0;
//};