#include "ast.h"

#include <stdlib.h>

void ast_node_free(struct ast_node node) {
	switch (node.category) {
		case AST_CATEGORY_OPERAND:
			for (size_t i = 0; i < node.operand.child_count; i++) {
				ast_node_free(node.operand.children[i]);
			}
			free(node.operand.children);
			break;
		case AST_CATEGORY_LITERAL:
			if (node.type == AST_TYPE_IDENTIFIER) {
				free(node.literal.identifier);
			}
			break;
		default:
			break;
	}
}

char *node_type_to_string(enum ast_node_type type) {
	switch (type) {
		case AST_TYPE_INTEGER_LITERAL:
			return "integer";
		case AST_TYPE_IDENTIFIER:
			return "identifier";
		case AST_TYPE_PROMOTE:
			return "promote";
		case AST_TYPE_NEGATE:
			return "negate";
		case AST_TYPE_INDEX:
			return "index";
		case AST_TYPE_ADD:
			return "add";
		case AST_TYPE_SUBTRACT:
			return "subtract";
		case AST_TYPE_MULTIPLY:
			return "multiply";
		case AST_TYPE_DIVIDE:
			return "divide";
		case AST_TYPE_ASSIGN:
			return "assign";
		case AST_TYPE_MEMBER:
			return "member";
		case AST_TYPE_TERNARY:
			return "ternary";
		case AST_TYPE_FUNCTION_CALL:
			return "function call";
		default:
			return "unknown";
	}
}

void operand_add_child(struct operand *operand, struct ast_node child) {
	operand->children = reallocarray(
	    operand->children, operand->child_count + 1, sizeof(struct ast_node)
	);
	operand->children[operand->child_count++] = child;
}
