#ifndef TEST_MODULE_H
#define TEST_MODULE_H
#include <functional>
#include <string>
#include <vector>
#include <any>

struct test {
    std::string name;
    std::string message; // for failed tests
    bool passed = false;
};

// test result
struct test_res {
    std::string test_name;
    std::string test_desc;
    std::vector<test*> tests = {};
    bool passed = true;

    template<typename... Args>
    test_res* add_tests(Args... new_tests) {
        std::initializer_list<test*>{new_tests...};
        for(auto& test : {new_tests...}) {
            if(test != nullptr) {
                if(!test->passed)
                    passed = false;

                this->tests.push_back(test);
            }
        }
        return this;
    }
    ~test_res() {
        for(auto& test : tests) {
            delete test;
        }
    }

};

// test module
struct test_mod {
    std::vector<std::function<test_res*()>> tests = {};
    std::vector<test_res*> results = {};
    std::string module_name;
    bool passed = false;

};
#endif //TEST_MODULE_H
