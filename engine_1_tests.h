#ifndef ENGINE_1_TESTING_LIBRARY_H
#define ENGINE_1_TESTING_LIBRARY_H

#include <utility>

#include "test_mod.h"

namespace tests {

    void DELETE_MODULE(test_mod* mod) {
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
        test_mod* mod = new test_mod;
        mod->module_name = std::move(name);
        (mod->tests.push_back(tests), ...);
        return mod;
    }

    inline void VERIFY_TEST_RESULTS(test_mod mod) {
        int passed = 0;
        int failed = 0;
        for(const auto& res : mod.results) {
            std::cout << res->test_name << std::endl;
            if(!res->passed) {
                std::cout << "High-level Test " << res->test_name << " failed" << std::endl;
                std::cout << "High-level Test description: " << res->test_desc << std::endl;
            }
            else {
                std::cout << "Test " << res->test_name << " passed" << std::endl;
                std::cout << "Test description: " << res->test_desc << std::endl;
            }
            std::cout << "Tests: " << std::endl;
            for (const auto &test : res->tests) {
                if (!test->passed) {
                    std::cout << "- Test " << test->name << " failed" << std::endl;
                    std::cout << "  Test message: " << test->message << std::endl;
                    failed++;
                }
                else {
                    std::cout << "+ Test " << test->name << " passed" << std::endl;
                    passed++;
                }
            }
        }
        std::cout << "High-level Test " << mod.module_name << " passed: " << passed << std::endl;
        std::cout << "High-level Test " << mod.module_name << " failed: " << failed << std::endl;
    }

    inline void VERIFY_MODULE_RESULTS(const std::vector<test_mod>& modules) {
        std::cout << "Test result for modules" << std::endl;
        int passed = 0;
        int failed = 0;
        for (const auto& mod : modules) {
            std::cout << "------------------------" << std::endl <<"START MODULE: " << mod.module_name << std::endl << "------------------------" << std::endl;
            if (!mod.passed) {
                std::cout << "Module " << mod.module_name << " failed" << std::endl;
                failed++;
            }
            else {
                std::cout << "Module " << mod.module_name << " passed" << std::endl;
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
    test* TEST_GREATER_THAN(T data, T expected, std::string name);

    template<typename T>
    test* TEST_LESS_THAN(T data, T expected, std::string name);

    template<typename T>
    test* TEST_GREATER_THAN_OR_EQUAL(T data, T expected, std::string name);

    template<typename T>
    test* TEST_LESS_THAN_OR_EQUAL(T data, T expected, std::string name);

    template<typename T>
    test* TEST_TRUE(T data, std::string name);

    template<typename T>
    test* TEST_FALSE(T data, std::string name);

    template<typename T>
    test* TEST_NULL(T data, std::string name);

    template<typename T>
    test* TEST_NOT_NULL(T data, std::string name);

    template<typename T>
    test* TEST_USER_INPUT_REQUIRED(std::string msg, std::string success_msg, std::string fail_msg, std::string name);
}


#endif //ENGINE_1_TESTING_LIBRARY_H
