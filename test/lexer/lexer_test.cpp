#include <gtest/gtest.h>

extern "C" {
#include <lexer/lexer.h>
}

TEST(lexer_test, create_lexer_should_return_valid_pointer) {
	Lexer *lexer = lexer_create("");
	ASSERT_NE(lexer, nullptr);
}

TEST(lexer_test, create_lexer_with_null_should_return_null) {
	Lexer *lexer = lexer_create(nullptr);
	ASSERT_EQ(lexer, nullptr);
}
