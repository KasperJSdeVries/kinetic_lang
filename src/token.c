#include "token.h"

#include <stdlib.h>
#include <string.h>

char *token_type_to_string(enum token_type type) {
	switch (type) {
		case TOKEN_PLUS:
			return "PLUS";
		case TOKEN_DASH:
			return "DASH";
		case TOKEN_STAR:
			return "STAR";
		case TOKEN_SLASH:
			return "SLASH";
		case TOKEN_EQUALS:
			return "EQUALS";
		case TOKEN_LEFT_PAREN:
			return "LEFT_PAREN";
		case TOKEN_RIGHT_PAREN:
			return "RIGHT_PAREN";
		case TOKEN_LEFT_BRACKET:
			return "LEFT_BRACKET";
		case TOKEN_RIGHT_BRACKET:
			return "RIGHT_BRACKET";
		case TOKEN_DOT:
			return "DOT";
		case TOKEN_COMMA:
			return "COMMA";
		case TOKEN_BANG:
			return "BANG";
		case TOKEN_QUESTION:
			return "QUESTION";
		case TOKEN_COLON:
			return "COLON";
		case TOKEN_SEMICOLON:
			return "SEMICOLON";
		case TOKEN_NUMBER:
			return "NUMBER";
		case TOKEN_IDENTIFIER:
			return "IDENTIFIER";
		default:
			return "unknown";
	}
}

char *lexeme_to_string(struct lexeme lexeme) {
	char *str = malloc(lexeme.length + 1);
	memcpy(str, lexeme.start, lexeme.length);
	str[lexeme.length] = '\0';
	return str;
}
