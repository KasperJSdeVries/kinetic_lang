#ifndef _LEXER_H
#define _LEXER_H

#include "token.h"

#include <stddef.h>

struct lexer {
	struct token *tokens;
	size_t token_count;
	size_t token_capacity;
};

struct lexer lexer_new(char *input);
void lexer_free(struct lexer *lexer);

struct token lexer_next(struct lexer *l);
struct token lexer_peek(struct lexer *l);

#endif // _LEXER_H
