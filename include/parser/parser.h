#ifndef _KINETIC_LANG_PARSER_H
#define _KINETIC_LANG_PARSER_H

#include <lexer/token.h>
#include <parser/ast.h>

typedef struct Parser_t Parser;

Parser *parser_create(Token tokens[]);
Module *parser_parse(Parser *parser);
void parser_free(Parser *parser);

#endif // _KINETIC_LANG_PARSER_H
