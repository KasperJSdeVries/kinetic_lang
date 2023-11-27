#include <lexer/lexer.h>

// std
#include <stdlib.h>

struct Lexer_t {
	const char *input;

	size_t current_index;
	size_t line_start;

	Token *tokens;
};

Lexer *lexer_create(const char *input) {
	if (input == NULL) {
		return NULL;
	}

	Lexer *lexer = malloc(sizeof(struct Lexer_t));

	// TODO: Implement this

	return lexer;
}

Token *lexer_lex(Lexer *lexer, size_t *out_token_count) {
	// TODO: Implement this
	return NULL;
}

void lexer_free(Lexer *lexer) {
	// TODO: Expand as necessary
	free(lexer);
}
