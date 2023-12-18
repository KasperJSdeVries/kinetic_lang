#include <gtest/gtest.h>

extern "C" {
#include "lexer.h"
#include "token.h"
}

TEST(lexer, test_expression) {
	struct lexer lexer = lexer_new("1 + 2 * 3 ! + a");
	struct token token;

	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_NUMBER);
	ASSERT_STREQ(strndup(token.lexeme.start, token.lexeme.length), "1");
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_PLUS);
	ASSERT_STREQ(strndup(token.lexeme.start, token.lexeme.length), "+");
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_NUMBER);
	ASSERT_STREQ(strndup(token.lexeme.start, token.lexeme.length), "2");
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_STAR);
	ASSERT_STREQ(strndup(token.lexeme.start, token.lexeme.length), "*");
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_NUMBER);
	ASSERT_STREQ(strndup(token.lexeme.start, token.lexeme.length), "3");
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_BANG);
	ASSERT_STREQ(strndup(token.lexeme.start, token.lexeme.length), "!");
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_PLUS);
	ASSERT_STREQ(strndup(token.lexeme.start, token.lexeme.length), "+");
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_IDENTIFIER);
	ASSERT_STREQ(strndup(token.lexeme.start, token.lexeme.length), "a");
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_EOF);
}
