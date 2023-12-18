#include "lexer.h"
#include "token.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static void lexer_lex(struct lexer *l);

static void lexer_add_token(struct lexer *l, struct token t);
static void lexer_add_single_character_token(
    struct lexer *l,
    enum token_type t
);

static char lexer_get_current_char(const struct lexer *l);
static void lexer_next_char(struct lexer *l);
static char lexer_peek_char(const struct lexer *l);
static const char *lexer_get_current_lexeme(const struct lexer *l);

struct lexer lexer_new(const char *input) {
	struct lexer l;
	l.input = input;
	l.current_index = 0;

	l.token_capacity = 8;
	l.token_count = 0;
	l.tokens = malloc(sizeof(struct token) * l.token_capacity);

	lexer_lex(&l);

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

static void lexer_lex(struct lexer *l) {
	while (lexer_get_current_char(l) != '\0') {
		while (isspace(lexer_get_current_char(l))) {
			lexer_next_char(l);
		}

		if (isalpha(lexer_get_current_char(l))) {
			const char *lexeme_start = lexer_get_current_lexeme(l);
			char c;
			size_t i = 1;
			while ((c = lexer_peek_char(l)) != '\0' && (isalnum(c) || c == '_')
			) {
				lexer_next_char(l);
				i++;
			}

			lexer_add_token(
			    l,
			    (struct token){
			        .type = TOKEN_IDENTIFIER,
			        .category = TOKEN_CATEGORY_LITERAL,
			        .lexeme =
			            (struct lexeme){
			                lexeme_start,
			                i,
			            },
			    }
			);
		} else if (isdigit(lexer_get_current_char(l))) {
			const char *lexeme_start = lexer_get_current_lexeme(l);
			char c;
			size_t i = 1;
			while ((c = lexer_peek_char(l)) != '\0' && (isdigit(c))) {
				lexer_next_char(l);
				i++;
			}

			lexer_add_token(
			    l,
			    (struct token){
			        .type = TOKEN_NUMBER,
			        .category = TOKEN_CATEGORY_LITERAL,
			        .lexeme =
			            (struct lexeme){
			                lexeme_start,
			                i,
			            },
			    }
			);
		} else {
			switch (lexer_get_current_char(l)) {
				case '+':
					lexer_add_single_character_token(l, TOKEN_PLUS);
					break;
				case '-':
					lexer_add_single_character_token(l, TOKEN_DASH);
					break;
				case '*':
					lexer_add_single_character_token(l, TOKEN_STAR);
					break;
				case '/':
					lexer_add_single_character_token(l, TOKEN_SLASH);
					break;
				case '=':
					lexer_add_single_character_token(l, TOKEN_EQUALS);
					break;
				case '!':
					lexer_add_single_character_token(l, TOKEN_BANG);
					break;
				case '?':
					lexer_add_single_character_token(l, TOKEN_QUESTION);
					break;
				case ':':
					lexer_add_single_character_token(l, TOKEN_COLON);
					break;
				case '(':
					lexer_add_single_character_token(l, TOKEN_LEFT_PAREN);
					break;
				case ')':
					lexer_add_single_character_token(l, TOKEN_RIGHT_PAREN);
					break;
				case '[':
					lexer_add_single_character_token(l, TOKEN_LEFT_BRACKET);
					break;
				case ']':
					lexer_add_single_character_token(l, TOKEN_RIGHT_BRACKET);
					break;
				case '.':
					lexer_add_single_character_token(l, TOKEN_DOT);
					break;
			}
		}
	}
}

static void lexer_add_single_character_token(
    struct lexer *l,
    enum token_type t
) {
	lexer_add_token(
	    l,
	    (struct token){
	        .type = t,
	        .category = TOKEN_CATEGORY_OPERATOR,
	        .lexeme =
	            (struct lexeme){
	                &l->input[l->current_index],
	                1,
	            },
	    }
	);
}

static void lexer_add_token(struct lexer *l, struct token t) {
	if (l->token_count + 1 > l->token_capacity) {
		l->token_capacity *= 2;
		l->tokens =
		    realloc(l->tokens, sizeof(struct token) * l->token_capacity);
	}
	l->tokens[l->token_count++] = t;
	lexer_next_char(l);
}

static char lexer_get_current_char(const struct lexer *l) {
	if (l->current_index > strlen(l->input)) {
		return '\0';
	}
	return l->input[l->current_index];
}

static void lexer_next_char(struct lexer *l) {
	l->current_index++;
}

static char lexer_peek_char(const struct lexer *l) {
	return l->input[l->current_index + 1];
}

static const char *lexer_get_current_lexeme(const struct lexer *l) {
	return &l->input[l->current_index];
}
