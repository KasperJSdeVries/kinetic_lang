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

TEST(lexer_test, lexing_whitespace_should_return_array_containing_TOKEN_EOF) {
	// Arrange
	Lexer *lexer = lexer_create(" \t\r\n ");
	size_t token_count;

	// Act
	Token *tokens = lexer_lex(lexer, &token_count);

	// Assert
	ASSERT_EQ(token_count, 1);
	ASSERT_EQ(tokens[0].type, TOKEN_EOF);
}

TEST(lexer_test, single_character_symbols_should_lex_correctly) {
	// Arrange
	Lexer *lexer = lexer_create("(){}[]:;.,_");
	size_t token_count;

	// Act
	Token *tokens = lexer_lex(lexer, &token_count);

	// Assert
	ASSERT_EQ(token_count, 12);
	ASSERT_EQ(tokens[0].type, TOKEN_LEFT_PAREN);
	ASSERT_EQ(tokens[1].type, TOKEN_RIGHT_PAREN);
	ASSERT_EQ(tokens[2].type, TOKEN_LEFT_BRACE);
	ASSERT_EQ(tokens[3].type, TOKEN_RIGHT_BRACE);
	ASSERT_EQ(tokens[4].type, TOKEN_LEFT_BRACKET);
	ASSERT_EQ(tokens[5].type, TOKEN_RIGHT_BRACKET);
	ASSERT_EQ(tokens[6].type, TOKEN_COLON);
	ASSERT_EQ(tokens[7].type, TOKEN_SEMICOLON);
	ASSERT_EQ(tokens[8].type, TOKEN_DOT);
	ASSERT_EQ(tokens[9].type, TOKEN_COMMA);
	ASSERT_EQ(tokens[10].type, TOKEN_UNDERSCORE);
	ASSERT_EQ(tokens[11].type, TOKEN_EOF);
}
