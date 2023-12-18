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

TEST(lexer, test_token_categories) {
	struct lexer lexer = lexer_new("1 2 3 bruh.+xl:*");
	struct token token;

	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_NUMBER);
	ASSERT_EQ(token.category, TOKEN_CATEGORY_LITERAL);
	ASSERT_STREQ(strndup(token.lexeme.start, token.lexeme.length), "1");
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_NUMBER);
	ASSERT_EQ(token.category, TOKEN_CATEGORY_LITERAL);
	ASSERT_STREQ(strndup(token.lexeme.start, token.lexeme.length), "2");
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_NUMBER);
	ASSERT_EQ(token.category, TOKEN_CATEGORY_LITERAL);
	ASSERT_STREQ(strndup(token.lexeme.start, token.lexeme.length), "3");
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_IDENTIFIER);
	ASSERT_EQ(token.category, TOKEN_CATEGORY_LITERAL);
	ASSERT_STREQ(strndup(token.lexeme.start, token.lexeme.length), "bruh");
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_DOT);
	ASSERT_EQ(token.category, TOKEN_CATEGORY_OPERATOR);
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_PLUS);
	ASSERT_EQ(token.category, TOKEN_CATEGORY_OPERATOR);
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_IDENTIFIER);
	ASSERT_EQ(token.category, TOKEN_CATEGORY_LITERAL);
	ASSERT_STREQ(strndup(token.lexeme.start, token.lexeme.length), "xl");
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_COLON);
	ASSERT_EQ(token.category, TOKEN_CATEGORY_OPERATOR);
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_STAR);
	ASSERT_EQ(token.category, TOKEN_CATEGORY_OPERATOR);
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_EOF);
	ASSERT_EQ(token.category, TOKEN_CATEGORY_UNKNOWN);
}
