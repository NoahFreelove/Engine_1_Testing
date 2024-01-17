#ifndef ENGINE_1_TESTING_LIBRARY_H
#define ENGINE_1_TESTING_LIBRARY_H

#include <utility>
#include <chrono>
#include "test_mod.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
namespace tests {
    inline void DELETE_MODULE(test_mod* mod) {
        for (test_res* res : mod->results) {
            delete res;
        }
        delete mod;
    }

    inline void DELETE_MODULES(std::vector<test_mod*> &modules) {
        for(auto &mod : modules) {
            DELETE_MODULE(mod);
        }
    }

    inline bool TEST_MODULE(test_mod* mod) {
        bool passed = true;
        for(const auto& test : mod->tests) {
            auto start = std::chrono::high_resolution_clock::now();
            auto res = test();
            auto end = std::chrono::high_resolution_clock::now();
            res->time_taken_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            res->time_taken_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            mod->results.push_back(res);
            if(!res->passed) {
                passed = false;
            }

        }
        mod->passed = passed;
        return passed;
    }

    inline void TEST_MODULES(std::vector<test_mod*> &modules) {
        for(auto &mod : modules){
            TEST_MODULE(mod);
        }
    }

    template<typename... Args>
    test_mod* GEN_MODULE(std::string name, Args... tests) {
        auto* mod = new test_mod;
        mod->module_name = std::move(name);
        (mod->tests.push_back(tests), ...);
        return mod;
    }

    inline void VERIFY_TEST_RESULTS(const test_mod& mod) {
        system(("chcp " + std::to_string(CP_UTF8) + " >nul").c_str());

        int passed = 0;
        int failed = 0;
        for(const auto& res : mod.results) {
            if(!res->passed) {
                std::cout << "\033[1;31m";
                std::cout << "- FUNCTION TEST: " << res->test_name << " FAILED" << std::endl;
                std::cout << "\033[0m";
                std::cout << "Test Description: " << res->test_desc << std::endl;
            }
            else {
                std::cout << "\033[1;32m";
                std::cout << "+ FUNCTION TEST: " << res->test_name << " PASSED" << std::endl;
                std::cout << "\033[0m";
                std::cout << "\033[1;37m";

                std::cout << "\033[0m";

                std::cout << "Test Description: \033[1;37m" << res->test_desc << "\033[0m" << std::endl;
            }
            std::cout << "    Sub-Tests: " << std::endl;
            for (const auto &test : res->tests) {
                if (!test->passed) {
                    std::cout << "    - \033[1;31mTest\033[1;31m '" << test->name << "' \033[1;31mfailed\033[1;31m" << std::endl;
                    std::cout << "        Fail message: " << test->message << std::endl;
                    failed++;
                }
                else {
                    // green
                    std::cout << "    + \033[1;32mTest\033[0m '" << test->name << "' \033[1;32mpassed\033[0m" << std::endl;
                    passed++;
                }
            }
            std::cout << "\033[1;30m";
            std::cout << "Time taken: " << res->time_taken_ns << "ns or " << res->time_taken_ms << "ms" << std::endl << std::endl;
            std::cout << "\033[0m";
        }
        std::cout << "\033[1;32m" << "Tests Passed: " << passed << ".\033[0m \033[1;31mTests Failed: " << failed << ".\033[0m" << std::endl;
        unsigned long long total_time_ns = 0;
        unsigned long long total_time_ms = 0;
        for(const auto& res : mod.results) {
            total_time_ns += res->time_taken_ns;
            total_time_ms += res->time_taken_ms;
        }
        std::cout << "Total time taken to test module: " << total_time_ns << "ns or " << total_time_ms << "ms" << std::endl;
    }

    inline void VERIFY_MODULE_RESULTS(const std::vector<test_mod>& modules) {
        std::cout << "Testing Results For " << modules.size() << " Modules..." << std::endl;
        int passed = 0;
        int failed = 0;
        system(("chcp " + std::to_string(CP_UTF8) + " >nul").c_str());

        for (const auto& mod : modules) {
            if (!mod.passed){
                std::cout << std::endl <<"\033[1;34mModule\033[0m: \033[1;33m" << mod.module_name << " \033[0m\033[1;31mFAILED\033[0m" << std::endl << std::endl;
                failed++;
            }
            else {
                std::cout << std::endl <<"\033[1;34mModule\033[0m: \033[1;33m" << mod.module_name << " \033[0m\033[1;32mPASSED\033[0m" << std::endl << std::endl;
                passed++;
            }
            VERIFY_TEST_RESULTS(mod);

            std::cout << "\033[1;34m";
            std::cout << std::endl << "End Module\033[0m: \033[1;33m" << mod.module_name << "\033[0m"<< std::endl << std::endl << std::endl;
        }
        std::cout << "Modules Passed: " << passed << std::endl;
        std::cout << "Modules Failed: " << failed << std::endl;
    }

    template<typename T>
    test* TEST_EQUAL(T data, T expected, std::string name) {
        const auto t = new test();
        t->name = std::move(name);
        if constexpr (std::is_same_v<decltype(data == expected), bool>) {
            t->passed = data == expected;
            if(!t->passed)
                t->message = "Unexpected value not equal to expected in an equal test";
            return t;
        }
        t->passed = false;
        t->message = "Cannot compare types " + std::string(typeid(data).name()) + " and " + std::string(typeid(expected).name());
        return t;

    }

    template<typename T>
    test *TEST_NOT_EQUAL(T data, T expected, std::string name) {
        const auto t = new test();
        t->name = std::move(name);
        if constexpr (std::is_same_v<decltype(data != expected), bool>) {
            t->passed = data != expected;
            if(!t->passed)
                t->message = "Expected value was equal to input value in a not equal test";
            return t;
        }
        t->passed = false;
        t->message = "Cannot compare types " + std::string(typeid(data).name()) + " and " + std::string(typeid(expected).name());
        return t;
    }

    template<typename T>
    test *TEST_GREATER_THAN(T val1, T val2, std::string name) {
        const auto t = new test();
        t->name = std::move(name);

        // make sure T supports > operator
        if constexpr (std::is_same_v<decltype(val1 > val2), bool>) {
            t->passed = val1 > val2;
            if(!t->passed)
                t->message = "Expected val1 to be greater than val 2";
            return t;
        }
        t->passed = false;
        t->message = "Cannot compare types " + std::string(typeid(val1).name()) + " and " + std::string(typeid(val2).name());
        return t;
    }

    template<typename T>
    test *TEST_LESS_THAN(T val1, T val2, std::string name) {
        const auto t = new test();
        t->name = std::move(name);

        // make sure T supports > operator
        if constexpr (std::is_same_v<decltype(val1 < val2), bool>) {
            t->passed = val1 < val2;
            if(!t->passed)
                t->message = "Expected val1 to be less than val 2";
            return t;
        }
        t->passed = false;
        t->message = "Cannot compare types " + std::string(typeid(val1).name()) + " and " + std::string(typeid(val2).name());
        return t;
    }

    template<typename T>
    test *TEST_GREATER_THAN_OR_EQUAL(T dal1, T val2, std::string name) {
        const auto t = new test();
        t->name = std::move(name);
        // make sure T supports >= operator
        if constexpr (std::is_same_v<decltype(dal1 >= val2), bool>) {
            t->passed = dal1 >= val2;
            if(!t->passed)
                t->message = "Expected val1 to be greater than or equal to val 2";
            return t;
        }
        t->passed = false;
        t->message = "Cannot compare types " + std::string(typeid(dal1).name()) + " and " + std::string(typeid(val2).name());
        return t;
    }

    template<typename T>
    test *TEST_LESS_THAN_OR_EQUAL(T val1, T val2, std::string name) {
        const auto t = new test();
        t->name = std::move(name);
        // make sure T supports <= operator
        if constexpr (std::is_same_v<decltype(val1 <= val2), bool>) {
            t->passed = val1 <= val2;
            if(!t->passed)
                t->message = "Expected val1 to be less than or equal to val 2";
            return t;
        }
        t->passed = false;
        t->message = "Cannot compare types " + std::string(typeid(val1).name()) + " and " + std::string(typeid(val2).name());
        return t;
    }

    template<typename T>
    test *TEST_TRUE(T data, std::string name) {
        const auto t = new test();
        t->name = std::move(name);
        // make sure T supports == operator
        if constexpr (std::is_same_v<decltype(data == true), bool>) {
            t->passed = data == true;
            if(!t->passed)
                t->message = "Expected input to be true";
            return t;
        }
        t->passed = false;
        t->message = "Cannot compare types " + std::string(typeid(data).name()) + " and bool";
        return t;
    }

    template<typename T>
    test *TEST_FALSE(T data, std::string name) {
        const auto t = new test();
        t->name = std::move(name);
        // make sure T supports == operator
        if constexpr (std::is_same_v<decltype(data == false), bool>) {
            t->passed = data == false;
            if(!t->passed)
                t->message = "Expected input to be false";
            return t;
        }
        t->passed = false;
        t->message = "Cannot compare types " + std::string(typeid(data).name()) + " and bool";
        return t;
    }

    template<typename T>
    test *TEST_NULL(T data, std::string name) {
        const auto t = new test();
        t->name = std::move(name);
        // make sure T supports == operator
        if constexpr (std::is_same_v<decltype(data == NULL), bool>) {
            t->passed = data == NULL;
            if(!t->passed)
                t->message = "Expected input to be null";
            return t;
        }
        t->passed = false;
        t->message = "Cannot compare types " + std::string(typeid(data).name()) + " and __null";
        return t;
    }

    template<typename T>
    test *TEST_NULL_PTR(T* data, std::string name) {
        const auto t = new test();
        t->name = std::move(name);
        // make sure T supports == operator
        if constexpr (std::is_same_v<decltype(data == nullptr), bool>) {
            t->passed = data == nullptr;
            if(!t->passed)
                t->message = "Expected input to be nullptr";
            return t;
        }
        t->passed = false;
        t->message = "Cannot compare types " + std::string(typeid(data).name()) + " and __null";
        return t;
    }

    template<typename T>
    test *TEST_NOT_NULL(T data, std::string name) {
        const auto t = new test();
        t->name = std::move(name);
        // make sure T supports == operator
        if constexpr (std::is_same_v<decltype(data != NULL), bool>) {
            t->passed = data != NULL;
            if(!t->passed)
                t->message = "Expected input data to not be null";
            return t;
        }
        t->passed = false;
        t->message = "Cannot compare types " + std::string(typeid(data).name()) + " and __null";
        return t;
    }

    inline test *TEST_USER_INPUT_REQUIRED(const std::string &msg, const std::string &success_msg, std::string fail_msg, std::string name) {
        const auto t = new test();
        t->name = std::move(name);
        std::cout << msg;
        // safely get user input and compare to expected
        std::string input;
        std::cin >> input;
        t->passed = input == success_msg;
        if(!t->passed)
            t->message = std::move(fail_msg);

        return t;
    }
    inline test *FAIL() {
        const auto t = new test();
        t->name = "FAIL TEST";
        t->message = "this was a purposeful fail test created with tests::FAIL()";
        t->passed = false;
        return t;
    }
    inline test *PASS() {
        const auto t = new test();
        t->name = "PASS TEST";
        t->passed = true;
        return t;
    }

}


#endif //ENGINE_1_TESTING_LIBRARY_H
