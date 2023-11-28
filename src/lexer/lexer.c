#include <lexer/lexer.h>
#include <lexer/token.h>

// std
#include <stdlib.h>

struct Lexer_t {
	const char *input;

	size_t current_index;
	size_t current_line;
	size_t line_start;

	Token *tokens;
	size_t tokens_length;
	size_t tokens_capacity;
};

void lexer_add_token(Lexer *lexer, Token token);
void lexer_add_single_char_token(Lexer *lexer, TokenType type);
char lexer_current(const Lexer *lexer);
char lexer_peek(const Lexer *lexer, int distance);
bool lexer_at_end(const Lexer *lexer);
void lexer_skip_whitespace(Lexer *lexer);

Lexer *lexer_create(const char *input) {
	if (input == NULL) {
		return NULL;
	}

	Lexer *lexer = malloc(sizeof(struct Lexer_t));

	lexer->current_index = 0;
	lexer->line_start = 0;
	lexer->input = input;

	lexer->tokens_capacity = 8;
	lexer->tokens = calloc(lexer->tokens_capacity, sizeof(Token));
	lexer->tokens_length = 0;

	return lexer;
}

Token *lexer_lex(Lexer *lexer, size_t *out_token_count) {
	while (!lexer_at_end(lexer)) {
		lexer_skip_whitespace(lexer);
	}

	lexer_add_single_char_token(lexer, TOKEN_EOF);
	*out_token_count = lexer->tokens_length;

	return lexer->tokens;
}

void lexer_free(Lexer *lexer) {
	// TODO: Expand as necessary
	free(lexer->tokens);
	free(lexer);
}

void lexer_add_token(Lexer *lexer, Token token) {
	if (lexer->tokens_length >= lexer->tokens_capacity) {
		lexer->tokens_capacity *= 2;
		lexer->tokens =
		    reallocarray(lexer->tokens, lexer->tokens_capacity, sizeof(Token));
	}

	lexer->tokens[lexer->tokens_length++] = token;
}

void lexer_add_single_char_token(Lexer *lexer, TokenType type) {
	lexer_add_token(
	    lexer,
	    token_create(
	        type, lexeme_create(lexer->input, lexer->current_index, 1),
	        position_create(
	            lexer->current_line, lexer->current_index - lexer->line_start
	        ),
	        position_create(
	            lexer->current_line, lexer->current_index - lexer->line_start
	        )
	    )
	);
}

char lexer_current(const Lexer *lexer) {
	return lexer->input[lexer->current_index];
}

char lexer_peek(const Lexer *lexer, int distance) {
	return lexer->input[lexer->current_index + distance];
}

bool lexer_at_end(const Lexer *lexer) {
	return lexer_current(lexer) == '\0';
}

void lexer_skip_whitespace(Lexer *lexer) {
	for (;;) {
		switch (lexer_peek(lexer, 1)) {
			case ' ':
			case '\r':
			case '\t':
				lexer->current_index++;
				break;
			case '\n':
				lexer->current_index++;
				lexer->line_start = lexer->current_index;
				lexer->current_line++;
				break;
			default:
				return;
		}
	}
}
