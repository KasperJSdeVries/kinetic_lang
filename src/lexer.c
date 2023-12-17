#include "lexer.h"
#include "token.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static void lexer_add_token(struct lexer *l, struct token t);

struct lexer lexer_new(const char *input) {
	struct lexer l;
	l.token_capacity = 8;
	l.token_count = 0;
	l.tokens = malloc(sizeof(struct token) * l.token_capacity);

	for (size_t i = 0; i < strlen(input); ++i) {
		char c = input[i];
		if (isspace(c)) {
			continue;
		} else if (isalnum(c)) {
			lexer_add_token(
			    &l, (struct token){.type = TOKEN_ATOM, .lexeme = c}
			);
		} else {
			lexer_add_token(&l, (struct token){.type = TOKEN_OP, .lexeme = c});
		}
	}

	for (size_t i = 0; i < l.token_count / 2; ++i) {
		struct token t = l.tokens[i];
		l.tokens[i] = l.tokens[l.token_count - i - 1];
		l.tokens[l.token_count - i - 1] = t;
	}

	return l;
}

void lexer_free(struct lexer *lexer) {
	free(lexer->tokens);
}

struct token lexer_next(struct lexer *l) {
	if (l->token_count < 1) {
		return (struct token){.type = TOKEN_EOF};
	}
	struct token t = l->tokens[l->token_count - 1];
	l->token_count--;
	return t;
}

struct token lexer_peek(struct lexer *l) {
	if (l->token_count < 1) {
		return (struct token){.type = TOKEN_EOF};
	}
	return l->tokens[l->token_count - 1];
}

static void lexer_add_token(struct lexer *l, struct token t) {
	if (l->token_count + 1 > l->token_capacity) {
		l->token_capacity *= 2;
		l->tokens =
		    realloc(l->tokens, sizeof(struct token) * l->token_capacity);
	}
	l->tokens[l->token_count++] = t;
}
