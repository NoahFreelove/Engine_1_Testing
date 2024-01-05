#ifndef ENGINE_1_TESTING_LIBRARY_H
#define ENGINE_1_TESTING_LIBRARY_H

#include <utility>

#include "test_mod.h"

namespace tests {

    inline void DELETE_MODULE(test_mod* mod) {
        for (test_res* res : mod->results) {
            delete res;
        }
        delete mod;
    }

    inline bool TEST_MODULE(test_mod &mod) {
        bool passed = true;
        for(const auto& test : mod.tests) {
            auto res = test();
            mod.results.push_back(res);
            if(!res->passed) {
                passed = false;
            }
        }
        mod.passed = passed;
        return passed;
    }

    inline void TEST_MODULES(std::vector<test_mod> &modules) {
        for(auto& mod : modules){
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
        int passed = 0;
        int failed = 0;
        for(const auto& res : mod.results) {
            if(!res->passed) {
                std::cout << "- FUNCTION TEST: " << res->test_name << " FAILED" << std::endl;
                std::cout << "Test Description: " << res->test_desc << std::endl;
            }
            else {
                std::cout << "+ FUNCTION TEST: " << res->test_name << " PASSED" << std::endl;
                std::cout << "Test Description: " << res->test_desc << std::endl;
            }
            std::cout << "    Sub-Tests: " << std::endl;
            for (const auto &test : res->tests) {
                if (!test->passed) {
                    std::cout << "    - Test '" << test->name << "' failed" << std::endl;
                    std::cout << "        Fail message: " << test->message << std::endl;
                    failed++;
                }
                else {
                    std::cout << "    + Test '" << test->name << "' passed" << std::endl;
                    passed++;
                }
            }
            std::cout << std::endl << std::endl;
        }
        std::cout << "Total Tests Passed: " << passed << " for module: " << mod.module_name << std::endl;
        std::cout << "Total Tests Failed: " << failed << " for module: " << mod.module_name  << std::endl;
    }

    inline void VERIFY_MODULE_RESULTS(const std::vector<test_mod>& modules) {
        std::cout << "Testing results for modules" << std::endl;
        int passed = 0;
        int failed = 0;
        for (const auto& mod : modules) {
            if (!mod.passed) {
            std::cout << "------------------------" << std::endl <<"- MODULE " << mod.module_name << " FAILED" << std::endl << "------------------------" << std::endl;
                failed++;
            }
            else {
            std::cout << "------------------------" << std::endl <<"+ MODULE " << mod.module_name << " PASSED" << std::endl << "------------------------" << std::endl;
                passed++;
            }
            VERIFY_TEST_RESULTS(mod);

            std::cout<< "------------------------" << std::endl << "END MODULE: " << mod.module_name << std::endl << "------------------------" << std::endl << std::endl;
        }
        std::cout << "Modules Passed: " << passed << std::endl;
        std::cout << "Modules Failed: " << failed << std::endl;
    }

    template<typename T>
    std::string to_string_helper(const T& t) {
        if constexpr (std::is_same_v<T, std::string>) {
            return t;
        } else {
            return std::to_string(t);
        }
    }

    template<typename T>
    test* TEST_EQUAL(T data, T expected, std::string name) {
        const auto t = new test();
        t->name = std::move(name);
        t->data = new std::any(data);
        t->expected = new std::any(expected);
        if constexpr (std::is_same_v<decltype(data == expected), bool>) {
            t->passed = data == expected;
            if(!t->passed)
                t->message = "Expected " + to_string_helper(expected) + " but got " + to_string_helper(data);
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
        t->data = new std::any(data);
        t->expected = new std::any(expected);
        if constexpr (std::is_same_v<decltype(data != expected), bool>) {
            t->passed = data != expected;
            if(!t->passed)
                t->message = "Expected anything BUT " + to_string_helper(expected) + " but got " + to_string_helper(data);
            return t;
        }
        t->passed = false;
        t->message = "Cannot compare types " + std::string(typeid(data).name()) + " and " + std::string(typeid(expected).name());
        return t;
    }

    template<typename T>
    test *TEST_GREATER_THAN(T data, T expected, std::string name) {
        const auto t = new test();
        t->name = std::move(name);
        t->data = new std::any(data);
        t->expected = new std::any(expected);
        // make sure T supports > operator
        if constexpr (std::is_same_v<decltype(data > expected), bool>) {
            t->passed = data > expected;
            if(!t->passed)
                t->message = "Expected " + to_string_helper(data) + " to be greater than " + to_string_helper(expected);
            return t;
        }
        t->passed = false;
        t->message = "Cannot compare types " + std::string(typeid(data).name()) + " and " + std::string(typeid(expected).name());
        return t;
    }

    template<typename T>
    test *TEST_LESS_THAN(T data, T expected, std::string name) {
        const auto t = new test();
        t->name = std::move(name);
        t->data = new std::any(data);
        t->expected = new std::any(expected);
        // make sure T supports > operator
        if constexpr (std::is_same_v<decltype(data < expected), bool>) {
            t->passed = data < expected;
            if(!t->passed)
                t->message = "Expected " + to_string_helper(data) + " to be greater than " + to_string_helper(expected);
            return t;
        }
        t->passed = false;
        t->message = "Cannot compare types " + std::string(typeid(data).name()) + " and " + std::string(typeid(expected).name());
        return t;
    }

    template<typename T>
    test *TEST_GREATER_THAN_OR_EQUAL(T data, T expected, std::string name) {
        const auto t = new test();
        t->name = std::move(name);
        t->data = new std::any(data);
        t->expected = new std::any(expected);
        // make sure T supports >= operator
        if constexpr (std::is_same_v<decltype(data >= expected), bool>) {
            t->passed = data >= expected;
            if(!t->passed)
                t->message = "Expected " + to_string_helper(data) + " to be greater than or equal to " + to_string_helper(expected);
            return t;
        }
        t->passed = false;
        t->message = "Cannot compare types " + std::string(typeid(data).name()) + " and " + std::string(typeid(expected).name());
        return t;
    }

    template<typename T>
    test *TEST_LESS_THAN_OR_EQUAL(T data, T expected, std::string name) {
        const auto t = new test();
        t->name = std::move(name);
        t->data = new std::any(data);
        t->expected = new std::any(expected);
        // make sure T supports <= operator
        if constexpr (std::is_same_v<decltype(data <= expected), bool>) {
            t->passed = data <= expected;
            if(!t->passed)
                t->message = "Expected " + to_string_helper(data) + " to be less than or equal to " + to_string_helper(expected);
            return t;
        }
        t->passed = false;
        t->message = "Cannot compare types " + std::string(typeid(data).name()) + " and " + std::string(typeid(expected).name());
        return t;
    }

    template<typename T>
    test *TEST_TRUE(T data, std::string name) {
        const auto t = new test();
        t->name = std::move(name);
        t->data = new std::any(data);
        t->expected = new std::any(true);
        // make sure T supports == operator
        if constexpr (std::is_same_v<decltype(data == true), bool>) {
            t->passed = data == true;
            if(!t->passed)
                t->message = "Expected " + to_string_helper(data) + " to be true";
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
        t->data = new std::any(data);
        t->expected = new std::any(true);
        // make sure T supports == operator
        if constexpr (std::is_same_v<decltype(data == false), bool>) {
            t->passed = data == false;
            if(!t->passed)
                t->message = "Expected " + to_string_helper(data) + " to be true";
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
        t->data = new std::any(data);
        t->expected = new std::any(NULL);
        // make sure T supports == operator
        if constexpr (std::is_same_v<decltype(data == NULL), bool>) {
            t->passed = data == NULL;
            if(!t->passed)
                t->message = "Expected " + to_string_helper(data) + " to be null";
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
        t->data = new std::any(data);
        t->expected = new std::any(NULL);
        // make sure T supports == operator
        if constexpr (std::is_same_v<decltype(data != NULL), bool>) {
            t->passed = data != NULL;
            if(!t->passed)
                t->message = "Expected " + to_string_helper(data) + " to not be null";
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
        t->data = nullptr;
        t->expected = nullptr;
        t->passed = false;
        return t;
    }
    inline test *PASS() {
        const auto t = new test();
        t->name = "PASS TEST";
        t->data = nullptr;
        t->expected = nullptr;
        t->passed = true;
        return t;
    }

}


#endif //ENGINE_1_TESTING_LIBRARY_H
