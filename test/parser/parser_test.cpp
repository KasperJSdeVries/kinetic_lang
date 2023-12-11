#include <gtest/gtest.h>

extern "C" {
#include <lexer/lexer.h>
#include <lexer/token.h>
#include <parser/parser.h>
}

TEST(parser_test, create_parser_should_return_valid_pointer) {
	// Arrange
	Position p = position_create(0, 0);
	Token tokens[] = {token_create(TOKEN_EOF, lexeme_create("", 0, 1), p, p)};

	// Act
	Parser *parser = parser_create(tokens);

	// Assert
	ASSERT_NE(parser, nullptr);
}

TEST(parser_test, create_parser_with_NULL_should_return_NULL) {
	// Act
	Parser *parser = parser_create(NULL);

	// Assert
	ASSERT_EQ(parser, nullptr);
}

TEST(
    parser_test,
    parser_parse_module_with_valid_input_should_return_valid_module
) {
	// Arrange
	Position p = position_create(0, 0);
	Token tokens[] = {token_create(TOKEN_EOF, lexeme_create("", 0, 1), p, p)};
	Parser *parser = parser_create(tokens);

	// Act
	Module *module = parser_parse_module(parser, "test");

	// Assert
	ASSERT_EQ(module->name.name, "test");
	ASSERT_EQ(module->declarations_count, 0);
}
