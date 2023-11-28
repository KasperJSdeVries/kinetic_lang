#include <assert/assert.h>
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
Position lexer_get_position(const Lexer *lexer);

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
	for (;;) {
		lexer_skip_whitespace(lexer);

		if (lexer_at_end(lexer)) {
			lexer_add_single_char_token(lexer, TOKEN_EOF);
			break;
		}

		switch (lexer_current(lexer)) {
			/***************************
			 * Single character tokens *
			 ***************************/
			case '(':
				lexer_add_single_char_token(lexer, TOKEN_LEFT_PAREN);
				lexer->current_index++;
				continue;
			case ')':
				lexer_add_single_char_token(lexer, TOKEN_RIGHT_PAREN);
				lexer->current_index++;
				continue;
			case '{':
				lexer_add_single_char_token(lexer, TOKEN_LEFT_BRACE);
				lexer->current_index++;
				continue;
			case '}':
				lexer_add_single_char_token(lexer, TOKEN_RIGHT_BRACE);
				lexer->current_index++;
				continue;
			case '[':
				lexer_add_single_char_token(lexer, TOKEN_LEFT_BRACKET);
				lexer->current_index++;
				continue;
			case ']':
				lexer_add_single_char_token(lexer, TOKEN_RIGHT_BRACKET);
				lexer->current_index++;
				continue;
			case ':':
				lexer_add_single_char_token(lexer, TOKEN_COLON);
				lexer->current_index++;
				continue;
			case ';':
				lexer_add_single_char_token(lexer, TOKEN_SEMICOLON);
				lexer->current_index++;
				continue;
			case '.':
				lexer_add_single_char_token(lexer, TOKEN_DOT);
				lexer->current_index++;
				continue;
			case ',':
				lexer_add_single_char_token(lexer, TOKEN_COMMA);
				lexer->current_index++;
				continue;
			case '_':
				lexer_add_single_char_token(lexer, TOKEN_UNDERSCORE);
				lexer->current_index++;
				continue;

			/**************************
			 * Multi character tokens *
			 **************************/
			case '!':
				if (lexer_peek(lexer, 1) == '=') {
					Position start = lexer_get_position(lexer);
					size_t start_index = lexer->current_index;
					lexer->current_index++;
					lexer_add_token(
					    lexer, token_create(
					               TOKEN_BANG_EQUALS,
					               lexeme_create(lexer->input, start_index, 2),
					               start, lexer_get_position(lexer)
					           )
					);
				} else {
					lexer_add_single_char_token(lexer, TOKEN_BANG);
				}
				lexer->current_index++;
				continue;
			case '=':
				if (lexer_peek(lexer, 1) == '=') {
					Position start = lexer_get_position(lexer);
					size_t start_index = lexer->current_index;
					lexer->current_index++;
					lexer_add_token(
					    lexer, token_create(
					               TOKEN_EQUALS_EQUALS,
					               lexeme_create(lexer->input, start_index, 2),
					               start, lexer_get_position(lexer)
					           )
					);
				} else {
					lexer_add_single_char_token(lexer, TOKEN_EQUALS);
				}
				lexer->current_index++;
				continue;
			case '+':
				if (lexer_peek(lexer, 1) == '=') {
					Position start = lexer_get_position(lexer);
					size_t start_index = lexer->current_index;
					lexer->current_index++;
					lexer_add_token(
					    lexer, token_create(
					               TOKEN_PLUS_EQUALS,
					               lexeme_create(lexer->input, start_index, 2),
					               start, lexer_get_position(lexer)
					           )
					);
				} else {
					lexer_add_single_char_token(lexer, TOKEN_PLUS);
				}
				lexer->current_index++;
				continue;
			case '-':
				if (lexer_peek(lexer, 1) == '=') {
					Position start = lexer_get_position(lexer);
					size_t start_index = lexer->current_index;
					lexer->current_index++;
					lexer_add_token(
					    lexer, token_create(
					               TOKEN_MINUS_EQUALS,
					               lexeme_create(lexer->input, start_index, 2),
					               start, lexer_get_position(lexer)
					           )
					);
				} else {
					lexer_add_single_char_token(lexer, TOKEN_MINUS);
				}
				lexer->current_index++;
				continue;
			case '*':
				if (lexer_peek(lexer, 1) == '=') {
					Position start = lexer_get_position(lexer);
					size_t start_index = lexer->current_index;
					lexer->current_index++;
					lexer_add_token(
					    lexer, token_create(
					               TOKEN_STAR_EQUALS,
					               lexeme_create(lexer->input, start_index, 2),
					               start, lexer_get_position(lexer)
					           )
					);
				} else {
					lexer_add_single_char_token(lexer, TOKEN_STAR);
				}
				lexer->current_index++;
				continue;
			case '/':
				if (lexer_peek(lexer, 1) == '=') {
					Position start = lexer_get_position(lexer);
					size_t start_index = lexer->current_index;
					lexer->current_index++;
					lexer_add_token(
					    lexer, token_create(
					               TOKEN_SLASH_EQUALS,
					               lexeme_create(lexer->input, start_index, 2),
					               start, lexer_get_position(lexer)
					           )
					);
				} else {
					lexer_add_single_char_token(lexer, TOKEN_SLASH);
				}
				lexer->current_index++;
				continue;
			default:
				ASSERT_NOT_REACHED();
		}
	}

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
	Position current_position = lexer_get_position(lexer);
	lexer_add_token(
	    lexer, token_create(
	               type, lexeme_create(lexer->input, lexer->current_index, 1),
	               current_position, current_position
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
		switch (lexer_current(lexer)) {
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

Position lexer_get_position(const Lexer *lexer) {
	return position_create(
	    lexer->current_line, lexer->current_index - lexer->line_start
	);
}
