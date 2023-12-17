#include <gtest/gtest.h>

extern "C" {
#include "lexer.h"
#include "token.h"
}

TEST(lexer, test_expression) {
	struct lexer lexer = lexer_new("1 + 2 * 3 !");
	struct token token;

	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_ATOM);
	ASSERT_EQ(token.lexeme, '1');
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_OP);
	ASSERT_EQ(token.lexeme, '+');
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_ATOM);
	ASSERT_EQ(token.lexeme, '2');
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_OP);
	ASSERT_EQ(token.lexeme, '*');
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_ATOM);
	ASSERT_EQ(token.lexeme, '3');
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_OP);
	ASSERT_EQ(token.lexeme, '!');
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_EOF);
}
