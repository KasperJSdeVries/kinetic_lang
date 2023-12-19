#ifndef _EXPRESSION_H
#define _EXPRESSION_H

#include "ast.h"

struct ast_node expression_parse(const char *input);

char *expression_format(const struct ast_node *expr);

#endif // _EXPRESSION_H
