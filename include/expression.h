#ifndef _EXPRESSION_H
#define _EXPRESSION_H

#include "ast.h"
#include "lexer.h"

struct ast_node expression_parse(struct lexer *lexer);

char *expression_format(const struct ast_node *expr);

#endif // _EXPRESSION_H
