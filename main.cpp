#include <iostream>
#include "gtest/gtest.h"
int Add(int a, int b) {
    return a + b;
}

TEST(testCase, test0) {
    EXPECT_EQ(14, Add(4, 10));
}
int main(int argc, char ** argv) {
    std::cout << "Hello, World!" << std::endl;
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
