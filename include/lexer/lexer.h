#ifndef _KINETIC_LANG_LEXER_H
#define _KINETIC_LANG_LEXER_H

#include "token.h"

typedef struct Lexer_t Lexer;

Lexer *lexer_create(const char *input);
Token *lexer_lex(Lexer *lexer, size_t *out_token_count);
void lexer_free(Lexer *lexer);

#endif // _KINETIC_LANG_LEXER_H
