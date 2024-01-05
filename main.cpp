#include <iostream>

#include "engine_1_tests.h"

test_res* test_test() {
    auto* res = new test_res();
    res->test_name = "test_test";
    res->test_desc = "This is a test";

    test* bool_test = tests::TEST_NOT_EQUAL<std::string>("1", "2", "test not equal");
    test* bool_test2 = tests::TEST_EQUAL<bool>(true, true, "test equal");

    return res->add_tests(bool_test, bool_test2);
}

test_res* test_test2() {
    auto* res = new test_res();
    res->test_name = "test_test2";
    res->test_desc = "This is a test2";

    test* bool_test = tests::TEST_NOT_EQUAL<std::string>("1", "2", "test not equal");
    test* bool_test2 = tests::TEST_EQUAL<bool>(true, true, "test equal");

    return res->add_tests(bool_test, bool_test2);
}

int main(){
    test_mod* mod = tests::GEN_MODULE("Module", test_test, test_test2);
    tests::TEST_MODULE(*mod);
    std::cout << mod->results.size() << std::endl;
    tests::VERIFY_MODULE_RESULTS({*mod});

    tests::DELETE_MODULE(mod);
    return 0;
}