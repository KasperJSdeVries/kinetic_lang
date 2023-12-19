#ifndef _AST_H
#define _AST_H

#include <stddef.h>

enum ast_node_type {
	AST_TYPE_UNKNOWN,
	// Literals
	AST_TYPE_INTEGER_LITERAL,
	AST_TYPE_IDENTIFIER,
	// Unary
	AST_TYPE_PROMOTE,
	AST_TYPE_NEGATE,
	AST_TYPE_INDEX,
	// Binary
	AST_TYPE_ADD,
	AST_TYPE_SUBTRACT,
	AST_TYPE_MULTIPLY,
	AST_TYPE_DIVIDE,
	AST_TYPE_ASSIGN,
	AST_TYPE_MEMBER,
	// Ternary
	AST_TYPE_TERNARY,

	// Other
	AST_TYPE_FUNCTION_CALL,
};

enum ast_node_category {
	AST_CATEGORY_NONE,
	AST_CATEGORY_LITERAL,
	AST_CATEGORY_OPERAND,
};

union literal {
	int integer;
	char *identifier;
};

struct operand {
	struct ast_node *children;
	size_t child_count;
};

struct ast_node {
	enum ast_node_type type;
	enum ast_node_category category;
	union {
		union literal literal;
		struct operand operand;
	};
};

void ast_node_free(struct ast_node node);

char *node_type_to_string(enum ast_node_type type);

void operand_add_child(struct operand *operand, struct ast_node child);

#endif // _AST_H
