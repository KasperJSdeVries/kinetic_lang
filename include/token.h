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
	TOKEN_BANG,
	TOKEN_QUESTION,
	TOKEN_COLON,
	TOKEN_SEMICOLON,
	// Literals
	TOKEN_NUMBER,
	TOKEN_IDENTIFIER,
};

struct lexeme {
	const char *start;
	size_t length;
};

struct token {
	enum token_type type;
	struct lexeme lexeme;
};

char *lexeme_to_string(struct lexeme lexeme);

#endif // _TOKEN_H
