#include <gtest/gtest.h>
#include <pch.hpp>

using namespace pcre2cpp;

// FINDS MATCH AT ANY LOCATION IN STRING
TEST(MATCH_TESTS, BASIC_MATCH) {
	const regex expression("\\d+");

	EXPECT_TRUE(expression.match("2"));
	EXPECT_FALSE(expression.match("a"));
	EXPECT_TRUE(expression.match("a2"));
}

// FINDS MATCH AT ANY LOCATION IN STRING
TEST(MATCH_TESTS, BASIC_MATCH_UTF16) {
	const u16regex expression(u"\\d+");

	EXPECT_TRUE(expression.match(u"2"));
	EXPECT_FALSE(expression.match(u"a"));
	EXPECT_TRUE(expression.match(u"a2"));
}

// FINDS MATCH AT ANY LOCATION IN STRING
TEST(MATCH_TESTS, BASIC_MATCH_UTF32) {
	const u32regex expression(U"\\d+");

	EXPECT_TRUE(expression.match(U"2"));
	EXPECT_FALSE(expression.match(U"a"));
	EXPECT_TRUE(expression.match(U"a2"));
}

// FINDS MATCH AT ANY LOCATION IN STRING AND RETURNS RESULT
TEST(MATCH_TESTS, MATCH_WITH_RESULT) {
	const regex expression("\\d+");

	match_result result;
	EXPECT_TRUE(expression.match("aa2", result, 2));
	EXPECT_EQ(result.get_search_offset(), 2);
	EXPECT_EQ(result.get_result_value(), "2");
	EXPECT_EQ(result.get_result_global_offset(), 2);
	EXPECT_EQ(result.get_result_relative_offset(), 0);

	EXPECT_TRUE(expression.match("aa2", result));
	EXPECT_EQ(result.get_search_offset(), 0);
	EXPECT_EQ(result.get_result_value(), "2");
	EXPECT_EQ(result.get_result_global_offset(), 2);
	EXPECT_EQ(result.get_result_relative_offset(), 2);
}

// TRIES TO FIND MATCH AT GIVEN LOCATION
TEST(MATCH_TESTS, MATCH_AT_SPECIFIED_OFFSET) {
	const regex expression("\\d+");

	EXPECT_TRUE(expression.match_at("aa2", 3, 2));
	EXPECT_FALSE(expression.match_at("aa2", 3, 1));
}

// TRIES TO FIND MATCH AT GIVEN LOCATION AND RETURNS RESULT
TEST(MATCH_TESTS, MATCH_AT_SPECIFIED_OFFSET_WITH_RESULT) {
	const regex expression("\\d+");

	match_result result;
	EXPECT_TRUE(expression.match_at("aa2", 3, result, 2));
	EXPECT_EQ(result.get_search_offset(), 2);
	EXPECT_EQ(result.get_result_value(), "2");
	EXPECT_EQ(result.get_result_global_offset(), 2);
	EXPECT_EQ(result.get_result_relative_offset(), 0);

	EXPECT_FALSE(expression.match_at("aa2", 3, result, 1));
	EXPECT_EQ(result.get_search_offset(), 1);
	EXPECT_EQ(result.get_result_value(), "");
	EXPECT_EQ(result.get_result_global_offset(), match_result::bad_offset);
	EXPECT_EQ(result.get_result_relative_offset(), match_result::bad_offset);
}

// MATCHES AND TRIES TO GET RESULTS FROM SUBEXPRESSION
TEST(MATCH_TESTS, MATCH_WITH_INDEXED_SUBEXPRESSION) {
	const regex expression("(\\d+)(a)");

	match_result result;
	EXPECT_TRUE(expression.match("ab23a", result, 1));
	EXPECT_EQ(result.get_search_offset(), 1);
	EXPECT_EQ(result.get_result_value(), "23a");
	EXPECT_EQ(result.get_result_global_offset(), 2);
	EXPECT_EQ(result.get_result_relative_offset(), 1);
	EXPECT_EQ(result.get_sub_result_value(0), "23");
	EXPECT_EQ(result.get_sub_result_global_offset(0), 2);
	EXPECT_EQ(result.get_sub_result_relative_offset(0), 1);
	EXPECT_EQ(result.get_sub_result_in_result_offset(0), 0);
	EXPECT_EQ(result.get_sub_result_value(1), "a");
	EXPECT_EQ(result.get_sub_result_global_offset(1), 4);
	EXPECT_EQ(result.get_sub_result_relative_offset(1), 3);
	EXPECT_EQ(result.get_sub_result_in_result_offset(1), 2);

	EXPECT_EQ(result.get_sub_results().size(), 2);
	EXPECT_EQ(result.get_sub_results_global_offsets().size(), 2);
	EXPECT_EQ(result.get_sub_results_relative_offsets().size(), 2);
	EXPECT_EQ(result.get_sub_results_in_result_offsets().size(), 2);
	EXPECT_EQ(result.get_sub_results_values().size(), 2);
}

// MATCHES AND TRIES TO GET RESULTS FROM NAMED SUBEXPRESSION
TEST(MATCH_TESTS, MATCH_WITH_NAMED_SUBEXPRESSION) {
	const regex expression("(?<number>\\d+)(?<a>a)");

	match_result result;
	EXPECT_TRUE(expression.match("ab23a", result, 1));
	EXPECT_EQ(result.get_search_offset(), 1);
	EXPECT_EQ(result.get_result_value(), "23a");
	EXPECT_EQ(result.get_result_global_offset(), 2);
	EXPECT_EQ(result.get_result_relative_offset(), 1);
	EXPECT_EQ(result.get_sub_result_value("number"), "23");
	EXPECT_EQ(result.get_sub_result_global_offset("number"), 2);
	EXPECT_EQ(result.get_sub_result_relative_offset("number"), 1);
	EXPECT_EQ(result.get_sub_result_in_result_offset("number"), 0);
	EXPECT_EQ(result.get_sub_result_value("a"), "a");
	EXPECT_EQ(result.get_sub_result_global_offset("a"), 4);
	EXPECT_EQ(result.get_sub_result_relative_offset("a"), 3);
	EXPECT_EQ(result.get_sub_result_in_result_offset("a"), 2);

	EXPECT_EQ(result.get_sub_results().size(), 2);
	EXPECT_EQ(result.get_sub_results_global_offsets().size(), 2);
	EXPECT_EQ(result.get_sub_results_relative_offsets().size(), 2);
	EXPECT_EQ(result.get_sub_results_in_result_offsets().size(), 2);
	EXPECT_EQ(result.get_sub_results_values().size(), 2);
}

// RETURNS ALL MATCHES
TEST(MATCH_TESTS, MATCH_ALL) {
	std::vector<match_result> results;
	const regex expression("\\d+");

	EXPECT_TRUE(expression.match_all("Ala ma 23 lata i 3 koty", 23, results));
	EXPECT_EQ(results.size(), 2);
	EXPECT_EQ(results[0].get_search_offset(), 0);
	EXPECT_EQ(results[0].get_result_global_offset(), 7);
	EXPECT_EQ(results[0].get_result_relative_offset(), 7);
	EXPECT_EQ(results[0].get_result_value(), "23");
	EXPECT_EQ(results[1].get_search_offset(), 0);
	EXPECT_EQ(results[1].get_result_global_offset(), 17);
	EXPECT_EQ(results[1].get_result_relative_offset(), 17);
	EXPECT_EQ(results[1].get_result_value(), "3");

	match_result* resultsPtr = nullptr;
	size_t resultsCount		 = 0;
	EXPECT_TRUE(expression.match_all("Ala ma 23 lata i 3 koty", 23, resultsPtr, resultsCount));
	EXPECT_EQ(resultsCount, 2);
	EXPECT_EQ(results[0].get_search_offset(), 0);
	EXPECT_EQ(results[0].get_result_global_offset(), 7);
	EXPECT_EQ(results[0].get_result_relative_offset(), 7);
	EXPECT_EQ(results[0].get_result_value(), "23");
	EXPECT_EQ(results[1].get_search_offset(), 0);
	EXPECT_EQ(results[1].get_result_global_offset(), 17);
	EXPECT_EQ(results[1].get_result_relative_offset(), 17);
	EXPECT_EQ(results[1].get_result_value(), "3");

	delete[] resultsPtr;
}

// TEST COPYING
TEST(REGEX_COPY, EXPRESSION_COPY) {
	const regex* expression		= new regex("\\d+");
	const regex otherExpression = *expression;
	delete expression;

	EXPECT_TRUE(otherExpression.match("2"));
}

// TEST COPYING SUB_VALUES_TABLE
TEST(REGEX_COPY, SUB_VALUES_TABLE_COPY) {
	const regex* expression = new regex("(?<number>\\d+)");

	match_result result;
	EXPECT_TRUE(expression->match("2", result));
	delete expression;

	EXPECT_EQ(result.get_search_offset(), 0);
	EXPECT_EQ(result.get_result_value(), "2");
	EXPECT_EQ(result.get_result_global_offset(), 0);
	EXPECT_EQ(result.get_result_relative_offset(), 0);
	EXPECT_EQ(result.get_sub_result_value("number"), "2");
	EXPECT_EQ(result.get_sub_result_global_offset("number"), 0);
	EXPECT_EQ(result.get_sub_result_relative_offset("number"), 0);
	EXPECT_EQ(result.get_sub_result_in_result_offset("number"), 0);

	EXPECT_EQ(result.get_sub_results().size(), 1);
	EXPECT_EQ(result.get_sub_results_global_offsets().size(), 1);
	EXPECT_EQ(result.get_sub_results_relative_offsets().size(), 1);
	EXPECT_EQ(result.get_sub_results_in_result_offsets().size(), 1);
	EXPECT_EQ(result.get_sub_results_values().size(), 1);
}