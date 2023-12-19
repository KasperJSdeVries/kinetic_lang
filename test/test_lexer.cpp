#include <gtest/gtest.h>

extern "C" {
#include "lexer.h"
#include "token.h"
}

TEST(lexer, test_expression) {
	struct lexer lexer = lexer_new("1 + 2 * 3 ! + a");
	struct token token;

	char *lexeme;
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_NUMBER);
	lexeme = lexeme_to_string(token.lexeme);
	ASSERT_STREQ(lexeme, "1");
	free(lexeme);

	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_PLUS);
	lexeme = lexeme_to_string(token.lexeme);
	ASSERT_STREQ(lexeme, "+");
	free(lexeme);

	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_NUMBER);
	lexeme = lexeme_to_string(token.lexeme);
	ASSERT_STREQ(lexeme, "2");
	free(lexeme);

	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_STAR);
	lexeme = lexeme_to_string(token.lexeme);
	ASSERT_STREQ(lexeme, "*");
	free(lexeme);

	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_NUMBER);
	lexeme = lexeme_to_string(token.lexeme);
	ASSERT_STREQ(lexeme, "3");
	free(lexeme);

	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_BANG);
	lexeme = lexeme_to_string(token.lexeme);
	ASSERT_STREQ(lexeme, "!");
	free(lexeme);

	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_PLUS);
	lexeme = lexeme_to_string(token.lexeme);
	ASSERT_STREQ(lexeme, "+");
	free(lexeme);

	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_IDENTIFIER);
	lexeme = lexeme_to_string(token.lexeme);
	ASSERT_STREQ(lexeme, "a");
	free(lexeme);

	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_EOF);

	lexer_free(&lexer);
}

TEST(lexer, test_token_categories) {
	struct lexer lexer = lexer_new("1 2 3 bruh.+xl:*");
	struct token token;
	char *lexeme;

	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_NUMBER);
	ASSERT_EQ(token.category, TOKEN_CATEGORY_LITERAL);
	lexeme = lexeme_to_string(token.lexeme);
	ASSERT_STREQ(lexeme, "1");
	free(lexeme);

	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_NUMBER);
	ASSERT_EQ(token.category, TOKEN_CATEGORY_LITERAL);
	lexeme = lexeme_to_string(token.lexeme);
	ASSERT_STREQ(lexeme, "2");
	free(lexeme);

	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_NUMBER);
	ASSERT_EQ(token.category, TOKEN_CATEGORY_LITERAL);
	lexeme = lexeme_to_string(token.lexeme);
	ASSERT_STREQ(lexeme, "3");
	free(lexeme);

	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_IDENTIFIER);
	ASSERT_EQ(token.category, TOKEN_CATEGORY_LITERAL);
	lexeme = lexeme_to_string(token.lexeme);
	ASSERT_STREQ(lexeme, "bruh");
	free(lexeme);

	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_DOT);
	ASSERT_EQ(token.category, TOKEN_CATEGORY_OPERATOR);

	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_PLUS);
	ASSERT_EQ(token.category, TOKEN_CATEGORY_OPERATOR);

	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_IDENTIFIER);
	ASSERT_EQ(token.category, TOKEN_CATEGORY_LITERAL);
	lexeme = lexeme_to_string(token.lexeme);
	ASSERT_STREQ(lexeme, "xl");
	free(lexeme);

	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_COLON);
	ASSERT_EQ(token.category, TOKEN_CATEGORY_OPERATOR);

	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_STAR);
	ASSERT_EQ(token.category, TOKEN_CATEGORY_OPERATOR);

	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_EOF);
	ASSERT_EQ(token.category, TOKEN_CATEGORY_UNKNOWN);

	lexer_free(&lexer);
}
