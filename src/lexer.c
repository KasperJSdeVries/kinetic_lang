#include "lexer.h"
#include "token.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void lexer_lex(struct lexer *l);

static void lexer_add_token(
    struct lexer *l,
    enum token_type type,
    enum token_category category
);
static void lexer_add_single_character_token(
    struct lexer *l,
    enum token_type t
);

static char lexer_get_current_char(const struct lexer *l);
static char lexer_next_char(struct lexer *l);
static enum token_type lexer_get_identifier_type(struct lexer *l);

struct lexer lexer_new(const char *input) {
	struct lexer l;
	l.input = input;

	l.lexeme_start = 0;
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

		l->lexeme_start = l->current_index;

		if (isalpha(lexer_get_current_char(l))) {
			char c;
			while ((c = lexer_next_char(l)) != '\0' && (isalnum(c) || c == '_'))
				;

			enum token_type keyword = lexer_get_identifier_type(l);

			lexer_add_token(l, keyword, TOKEN_CATEGORY_LITERAL);
		} else if (isdigit(lexer_get_current_char(l))) {
			char c;
			while ((c = lexer_next_char(l)) != '\0' && (isdigit(c)))
				;

			lexer_add_token(l, TOKEN_NUMBER, TOKEN_CATEGORY_LITERAL);
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
				case ',':
					lexer_add_single_character_token(l, TOKEN_COMMA);
					break;
			}
		}
	}
}

static void lexer_add_single_character_token(
    struct lexer *l,
    enum token_type t
) {
	lexer_next_char(l);
	lexer_add_token(l, t, TOKEN_CATEGORY_OPERATOR);
}

static void lexer_add_token(
    struct lexer *l,
    enum token_type type,
    enum token_category category
) {
	if (l->token_count + 1 > l->token_capacity) {
		l->token_capacity *= 2;
		l->tokens =
		    reallocarray(l->tokens, l->token_capacity, sizeof(struct token));
	}

	struct token t = {
	    .type = type,
	    .category = category,
	    .lexeme =
	        (struct lexeme){
	            &l->input[l->lexeme_start],
	            l->current_index - l->lexeme_start,
	        },
	};

	l->tokens[l->token_count++] = t;
}

static char lexer_get_current_char(const struct lexer *l) {
	if (l->current_index > strlen(l->input)) {
		return '\0';
	}
	return l->input[l->current_index];
}

static char lexer_next_char(struct lexer *l) {
	return l->input[++l->current_index];
}

static enum token_type lexer_check_keyword(
    struct lexer *l,
    size_t start,
    size_t length,
    const char *rest,
    enum token_type type
) {
	if (l->current_index - l->lexeme_start == start + length &&
	    memcmp(&l->input[l->lexeme_start + start], rest, length) == 0) {
		return type;
	}
	return TOKEN_IDENTIFIER;
}

static enum token_type lexer_get_identifier_type(struct lexer *l) {
	// Use a trie to search for keywords
	switch (l->input[l->lexeme_start]) {
		case 'c':
			return lexer_check_keyword(l, 1, 4, "onst", TOKEN_CONST);
		case 'd':
			return lexer_check_keyword(l, 1, 1, "o", TOKEN_DO);
		case 'e':
			return lexer_check_keyword(l, 1, 3, "lse", TOKEN_ELSE);
		case 'f':
			if (l->current_index - l->lexeme_start > 1) {
				switch (l->input[l->lexeme_start + 1]) {
					case 'a':
						return lexer_check_keyword(l, 2, 3, "lse", TOKEN_FALSE);
					case 'o':
						return lexer_check_keyword(l, 2, 1, "r", TOKEN_FOR);
				}
			}
			break;
		case 'i':
			if (l->current_index - l->lexeme_start > 1) {
				switch (l->input[l->lexeme_start + 1]) {
					case 'f':
						if (l->current_index - l->lexeme_start == 2) {
							return TOKEN_IF;
						}
						break;
					case 'n':
						if (l->current_index - l->lexeme_start > 2) {
							return lexer_check_keyword(
							    l, 2, 7, "terface", TOKEN_INTERFACE
							);
						} else {
							return TOKEN_IN;
						}
						break;
				}
			}
			break;
		case 'm':
			return lexer_check_keyword(l, 1, 2, "ut", TOKEN_MUT);
		case 'r':
			return lexer_check_keyword(l, 1, 5, "eturn", TOKEN_RETURN);
		case 's':
			if (l->current_index - l->lexeme_start > 1) {
				switch (l->input[l->lexeme_start + 1]) {
					case 't':
						return lexer_check_keyword(
						    l, 2, 4, "ruct", TOKEN_STRUCT
						);
					case 'w':
						return lexer_check_keyword(
						    l, 2, 4, "itch", TOKEN_SWITCH
						);
				}
			}
			break;
		case 't':
			return lexer_check_keyword(l, 1, 3, "rue", TOKEN_TRUE);
		case 'w':
			return lexer_check_keyword(l, 1, 4, "hile", TOKEN_WHILE);
	}

	return TOKEN_IDENTIFIER;
}
