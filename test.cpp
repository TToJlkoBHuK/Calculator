#include <iostream>
#include <vector>
#include <map>
#include <exception>
#include <string>
#include <functional>
#include <memory>
#include <cassert>

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
    Ret(Object::* method)(Args...);
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

    int execute(const std::string& commandName, const std::map<std::string, int>& args) {
        if (commands.find(commandName) != commands.end()) {
            return commands[commandName]->execute(args);
        }
        throw std::runtime_error("Command not found");
    }
};

//Subject reciver
class Subject {
public:
    int val = 0;

    int f3(int a, int b) {
        return a + b;
    }

    int f_zero() {
        return 42;
    }

    int f_set_val(int v) {
        val = v;
        return val;
    }
};
void print_header(const std::string& testName) {
    std::cout << "\n========================================\n";
    std::cout << "RUNNING: " << testName << "\n";
    std::cout << "----------------------------------------\n";
}

void print_success() {
    std::cout << "RESULT: [OK] Passed\n";
}

void print_fail(const std::string& reason) {
    std::cout << "RESULT: [FAIL] " << reason << "\n";
    std::exit(1);
}

// --- UNIT TESTS ---

/**
 * UNIT TEST 0: base test main.
 */
void test_main_scenario() {
    print_header("Test 0: Main Scenario (Happy Path)");

    Subject subj;
    std::cout << "Step 1: Created Subject.\n";

    Wrapper wrapper(&subj, &Subject::f3, { {"arg1", 0}, {"arg2", 0} });
    std::cout << "Step 2: Created Wrapper for Subject::f3 with args definition.\n";

    Engine engine;
    engine.register_command(&wrapper, "command1");
    std::cout << "Step 3: Registered 'command1' in Engine.\n";

    std::cout << "Step 4: Executing 'command1' with {arg1=4, arg2=5}...\n";
    try {
        int result = engine.execute("command1", { {"arg1", 4}, {"arg2", 5} });
        std::cout << "   -> Return value: " << result << "\n";

        if (result == 9) {
            print_success();
        }
        else {
            print_fail("Expected 9, got " + std::to_string(result));
        }
    }
    catch (const std::exception& e) {
        print_fail("Unexpected exception: " + std::string(e.what()));
    }
}

/**
 * UNIT TEST 1: Incorrect number of arguments.
 */
void test_argument_count_mismatch() {
    print_header("Test 1: Argument Count Mismatch");

    Subject subj;
    Wrapper wrapper(&subj, &Subject::f3, { {"arg1", 0}, {"arg2", 0} });

    Engine engine;
    engine.register_command(&wrapper, "sum");

    std::cout << "Step 1: Attempting to execute 'sum' with only 1 argument (expected 2).\n";
    try {
        engine.execute("sum", { {"arg1", 10} });
        print_fail("Exception was expected but not thrown.");
    }
    catch (const std::runtime_error& e) {
        std::string msg = e.what();
        std::cout << "   -> Caught expected exception: " << msg << "\n";
        if (msg == "Arguments count mismatch") {
            print_success();
        }
        else {
            print_fail("Wrong exception message. Expected 'Arguments count mismatch'.");
        }
    }
    catch (...) {
        print_fail("Caught unknown exception type.");
    }
}

/**
 * UNIT TEST 2: Invalid argument keys.
 */
void test_wrong_argument_keys() {
    print_header("Test 2: Wrong Argument Keys");

    Subject subj;
    Wrapper wrapper(&subj, &Subject::f3, { {"arg1", 0}, {"arg2", 0} });

    Engine engine;
    engine.register_command(&wrapper, "sum");

    std::cout << "Step 1: Attempting to execute with keys 'x' and 'y' instead of 'arg1', 'arg2'.\n";
    try {
        engine.execute("sum", { {"x", 10}, {"y", 20} });
        print_fail("Exception was expected but not thrown.");
    }
    catch (const std::out_of_range& e) {
        std::cout << "   -> Caught expected std::out_of_range (map key missing).\n";
        print_success();
    }
    catch (const std::exception& e) {
        std::cout << "   -> Caught exception: " << e.what() << "\n";
        print_success();
    }
}

/**
 * UNIT TEST 3: Command not found.
 */
void test_command_not_found() {
    print_header("Test 3: Command Not Found");

    Engine engine;
    std::cout << "Step 1: Engine is empty. Executing 'ghost_command'.\n";

    try {
        engine.execute("ghost_command", {});
        print_fail("Exception was expected.");
    }
    catch (const std::runtime_error& e) {
        std::string msg = e.what();
        std::cout << "   -> Caught expected exception: " << msg << "\n";
        if (msg == "Command not found") {
            print_success();
        }
        else {
            print_fail("Wrong error message.");
        }
    }
}

/**
 * UNIT TEST 4: Function without arguments (Arity 0).
 */
void test_zero_arguments() {
    print_header("Test 4: Zero Arguments Function");

    Subject subj;
    Wrapper wrapper(&subj, &Subject::f_zero, {});

    Engine engine;
    engine.register_command(&wrapper, "get_42");

    std::cout << "Step 1: Executing command expecting 0 arguments.\n";
    try {
        int res = engine.execute("get_42", {});
        std::cout << "   -> Result: " << res << "\n";
        if (res == 42) {
            print_success();
        }
        else {
            print_fail("Expected 42.");
        }
    }
    catch (const std::exception& e) {
        print_fail("Exception thrown: " + std::string(e.what()));
    }
}

/**
 * UNIT TEST 5: Changing the state (State Modification).
 */
void test_state_modification() {
    print_header("Test 5: State Modification");

    Subject subj;
    subj.val = 0;
    std::cout << "Step 1: Subject initial val = " << subj.val << "\n";

    Wrapper wrapper(&subj, &Subject::f_set_val, { {"new_val", 0} });

    Engine engine;
    engine.register_command(&wrapper, "set");

    std::cout << "Step 2: Executing 'set' with argument 100.\n";
    int res = engine.execute("set", { {"new_val", 100} });

    std::cout << "   -> Returned: " << res << "\n";
    std::cout << "   -> Subject val is now: " << subj.val << "\n";

    if (res == 100 && subj.val == 100) {
        print_success();
    }
    else {
        print_fail("State was not updated correctly.");
    }
}

int main() {
    try {
        test_main_scenario();
        test_argument_count_mismatch();
        test_wrong_argument_keys();
        test_command_not_found();
        test_zero_arguments();
        test_state_modification();

        std::cout << "\nALL TESTS PASSED SUCCESSFULLY.\n";
    }
    catch (...) {
        std::cerr << "CRITICAL FAILURE IN TEST RUNNER\n";
        return 1;
    }
    return 0;
}