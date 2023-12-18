#ifndef _EXPRESSION_H
#define _EXPRESSION_H

#include "token.h"
#include <stddef.h>

enum expression_type {
	E_ATOM,
	E_CONS,
};

struct e_atom {
	char *value;
};

struct e_cons {
	enum token_type op;
	struct expression *subexprs;
	size_t subexpr_count;
};

struct expression {
	enum expression_type type;
	union {
		struct e_atom atom;
		struct e_cons cons;
	};
};

struct expression expression_new(const char *input);
void expression_free(struct expression *expr);

char *expression_format(const struct expression *expr);

#endif // _EXPRESSION_H
