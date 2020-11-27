#include <gtest/gtest.h>

bool f()
{
    return false;
}

TEST(openPubSubTests, DemonstratedGTestMacros)
{
    EXPECT_EQ(true, true);
    const bool result = f();
    EXPECT_EQ(true, result) << "Hello, World";
}