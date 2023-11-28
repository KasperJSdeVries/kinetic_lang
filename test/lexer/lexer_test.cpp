#include <cstddef>
#include <gtest/gtest.h>

extern "C" {
#include <lexer/lexer.h>
#include <lexer/token.h>
}

TEST(lexer_test, create_lexer_should_return_valid_pointer) {
	// Act
	Lexer *lexer = lexer_create("");

	// Assert
	ASSERT_NE(lexer, nullptr);
}

TEST(lexer_test, create_lexer_with_null_should_return_null) {
	// Act
	Lexer *lexer = lexer_create(nullptr);

	// Assert
	ASSERT_EQ(lexer, nullptr);
}

TEST(lexer_test, lexing_empty_input_should_return_array_containing_TOKEN_EOF) {
	// Arrange
	Lexer *lexer = lexer_create("");
	size_t token_count;

	// Act
	Token *tokens = lexer_lex(lexer, &token_count);

	// Assert
	ASSERT_EQ(token_count, 1);
	ASSERT_EQ(tokens[0].type, TOKEN_EOF);
}
