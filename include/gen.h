#ifndef _GEN_H
#define _GEN_H

#include "ast.h"

#include <stdio.h>

void generatecode(struct ast_node n, FILE *out_file);

#endif // _GEN_H
