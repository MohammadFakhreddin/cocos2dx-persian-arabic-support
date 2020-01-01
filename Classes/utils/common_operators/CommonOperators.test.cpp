#include "gtest/gtest.h"
#include "CommonOperators.h"

TEST(CommonOperators, split) {
	auto case1Result = CommonOperators::split("mamad   reza masood", ' ');
	std::vector<std::string> case1ExpectedResult = { "mamad", "reza", "masood" };

	EXPECT_EQ(case1Result, case1ExpectedResult);
}

TEST(CommonOperators, trim) {
	auto case1Result = CommonOperators::trim("    mamad   reza masood     ");
	auto case1ExpectedResult = "mamad   reza masood";
	EXPECT_EQ(case1Result, case1ExpectedResult);
}
