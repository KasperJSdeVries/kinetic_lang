#ifndef _TOKEN_H
#define _TOKEN_H

#include <stddef.h>

enum token_type {
	TOKEN_EOF,
	// Operators
	TOKEN_PLUS,
	TOKEN_DASH,
	TOKEN_STAR,
	TOKEN_SLASH,
	TOKEN_EQUALS,
	TOKEN_LEFT_PAREN,
	TOKEN_RIGHT_PAREN,
	TOKEN_LEFT_BRACKET,
	TOKEN_RIGHT_BRACKET,
	TOKEN_DOT,
	TOKEN_COMMA,
	TOKEN_BANG,
	TOKEN_QUESTION,
	TOKEN_COLON,
	TOKEN_SEMICOLON,
	// Literals
	TOKEN_NUMBER,
	TOKEN_IDENTIFIER,
	// Keywords
	TOKEN_CONST,
	TOKEN_DO,
	TOKEN_ELSE,
	TOKEN_FALSE,
	TOKEN_FOR,
	TOKEN_IF,
	TOKEN_IN,
	TOKEN_INTERFACE,
	TOKEN_MUT,
	TOKEN_RETURN,
	TOKEN_STRUCT,
	TOKEN_SWITCH,
	TOKEN_TRUE,
	TOKEN_WHILE,
};

struct lexeme {
	const char *start;
	size_t length;
};

enum token_category {
	TOKEN_CATEGORY_UNKNOWN,
	TOKEN_CATEGORY_OPERATOR,
	TOKEN_CATEGORY_LITERAL,
};

struct token {
	enum token_type type;
	enum token_category category;
	struct lexeme lexeme;
};

char *token_type_to_string(enum token_type type);
char *lexeme_to_string(struct lexeme lexeme);

#endif // _TOKEN_H
