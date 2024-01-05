#include <iostream>

#include "engine_1_tests.h"

test_res* test_test() {
    auto* res = new test_res();
    res->test_name = "test_test";
    res->test_desc = "This is a test";

    test* bool_test = tests::TEST_NOT_EQUAL<std::string>("1", "2", "test not equal");
    test* bool_test2 = tests::TEST_EQUAL<bool>(true, true, "test equal");
    test* fail = tests::FAIL();
    return res->add_tests(bool_test, bool_test2, fail);
}

test_res* user_test() {
    auto* res = new test_res();
    res->test_name = "Get User Input";
    res->test_desc = "Gets user input and checks if its equal to Y";

    test* actual_test = tests::TEST_USER_INPUT_REQUIRED("Say Y: ", "Y", "Did not respond Y", "Y test");

    return res->add_tests(actual_test);
}

test_res* falsy() {
    auto* res = new test_res();
    res->test_name = "Falsey";
    res->test_desc = "False for fun";

    test* actual_test = tests::TEST_FALSE<bool>(false, "Check false boolean is false");

    return res->add_tests(actual_test);
}

test_res* truthy() {
    auto* res = new test_res();
    res->test_name = "Truthy";
    res->test_desc = "True for fun";

    test* actual_test = tests::TEST_TRUE<bool>(true, "Check true boolean is true");

    return res->add_tests(actual_test);
}

int main(){
    test_mod* mod = tests::GEN_MODULE("Module", test_test);

    auto mod_list = std::vector<test_mod>();
    mod_list.push_back(*mod);

    tests::TEST_MODULE(*mod);

    auto* mod2 = tests::GEN_MODULE("Truthy and Falsey", truthy, falsy);
    tests::TEST_MODULE(*mod2);

    //tests::TEST_MODULES(mod_list); // where the magic happens

    //std::cout << mod->results.size() << std::endl;
    tests::VERIFY_MODULE_RESULTS({*mod, *mod2});

    tests::DELETE_MODULE(mod);
    tests::DELETE_MODULE(mod2);
    return 0;
}