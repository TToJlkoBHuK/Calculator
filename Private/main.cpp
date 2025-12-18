#include <iostream>
#include <cassert>
#include "Engine.h"
#include "Wrapper.h"
#include "Subject.h"

// log message
void run_test(const std::string& testName, std::function<void()> testBody) {
    std::cout << "\n------------------------------------------------------------\n";
    std::cout << "TEST START: " << testName << "\n";
    std::cout << "------------------------------------------------------------\n";
    try {
        testBody();
        std::cout << ">>> TEST RESULT: [SUCCESS]\n";
    }
    catch (const std::exception& e) {
        std::cout << ">>> TEST RESULT: [FAILURE] Exception: " << e.what() << "\n";
    }
}

int main() {
    std::cout << "Initializing System..." << std::endl;

    Subject subj;
    Engine engine;

    std::cout << "[Main] Registering wrappers..." << std::endl;
    //test example presentation
    Wrapper wrapper_f3(&subj, &Subject::f3, { {"arg1", 0}, {"arg2", 0} });
    engine.register_command(&wrapper_f3, "command1");

    run_test("Standard 2-Argument Method", [&]() {
        std::cout << "   Action: Executing 'calc_sum' with {arg1=4, arg2=5}\n";
        int result = engine.execute("command1", { {"arg1", 4}, {"arg2", 5} });
        std::cout << "   Expected: 9\n";
        std::cout << "   Actual:   " << result << "\n";
        if (result != 9) throw std::logic_error("Calculation error");
    });

    //test any quantity args
    Wrapper wrapper_big(&subj, &Subject::sum_4_args, {
        {"a", 0}, {"b", 0}, {"c", 0}, {"d", 0}
        });
    engine.register_command(&wrapper_big, "sum4");

    run_test("Variadic Template (4 args)", [&]() {
        std::cout << "   Action: Executing 'sum4' with {a=1, b=2, c=3, d=4}\n";
        int result = engine.execute("sum4", {{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}});
        std::cout << "   Expected: 10\n";
        std::cout << "   Actual:   " << result << "\n";
        if (result != 10) throw std::logic_error("Calculation error");
    });

    //test zero args
    Wrapper wrapper_zero(&subj, &Subject::f_zero, {});
    engine.register_command(&wrapper_zero, "zero");

    run_test("Zero Arguments Method", [&]() {
        std::cout << "   Action: Executing 'zero' with {}\n";
        int result = engine.execute("zero", {});
        std::cout << "   Expected: 42\n";
        std::cout << "   Actual:   " << result << "\n";
        if (result != 42) throw std::logic_error("Value mismatch");
    });

    //test error
    run_test("Error Handling: Missing Argument Key", [&]() {
        std::cout << "   Action: Executing 'sum4' with missing 'arg2'\n";
        engine.execute("sum4", { {"arg1", 10}, {"argTypo", 20} });
    });

    //test error
    run_test("Error Handling: Wrong Argument Count", [&]() {
        std::cout << "   Action: Executing 'sum4' with only 1 argument\n";
        engine.execute("sum4", { {"arg1", 10} });
    });
    return 0;
}