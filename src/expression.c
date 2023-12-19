#include "expression.h"
#include "ast.h"
#include "lexer.h"
#include "token.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct binding_power {
	uint8_t left;
	uint8_t right;
	bool some;
};

static struct ast_node expression_binding_power(
    struct lexer *lexer,
    uint8_t min_bp
);
static struct binding_power prefix_binding_power(enum token_type op);
static struct binding_power infix_binding_power(enum token_type op);
static struct binding_power postfix_binding_power(enum token_type op);

static enum ast_node_type get_ast_node_type(enum token_type op);

struct ast_node expression_parse(const char *input) {
	struct lexer lexer = lexer_new(input);
	struct ast_node expr = expression_binding_power(&lexer, 0);
	lexer_free(&lexer);
	return expr;
}

char *expression_format(const struct ast_node *expr) {
	char *s = calloc(1024, 1);
	size_t offset = 0;
	switch (expr->category) {
		case AST_CATEGORY_LITERAL:
			switch (expr->type) {
				case AST_TYPE_INTEGER_LITERAL:
					sprintf(s + offset, "%d", expr->literal.integer);
					break;
				case AST_TYPE_IDENTIFIER:
					sprintf(s + offset, "%s", expr->literal.identifier);
					break;
				default:
					fprintf(stderr, "bad literal %d\n", expr->type);
					exit(1);
			}
			break;
		case AST_CATEGORY_OPERAND: {
			char *op;
			switch (expr->type) {
				case AST_TYPE_ADD:
					op = "+";
					break;
				case AST_TYPE_SUBTRACT:
					op = "-";
					break;
				case AST_TYPE_MULTIPLY:
					op = "*";
					break;
				case AST_TYPE_DIVIDE:
					op = "/";
					break;
				case AST_TYPE_ASSIGN:
					op = "=";
					break;
				case AST_TYPE_FUNCTION_CALL:
					op = "()";
					break;
				case AST_TYPE_INDEX:
					op = "[]";
					break;
				case AST_TYPE_MEMBER:
					op = ".";
					break;
				case AST_TYPE_PROMOTE:
					op = "+";
					break;
				case AST_TYPE_TERNARY:
					op = "?:";
					break;
				default:
					fprintf(
					    stderr, "bad op %s\n", node_type_to_string(expr->type)
					);
					exit(1);
			}
			sprintf(s + offset, "(%s", op);
			offset = strlen(s);
			for (size_t i = 0; i < expr->operand.child_count; i++) {

				char *child_str = expression_format(&expr->operand.children[i]);
				sprintf(s + offset, " %s", child_str);
				free(child_str);
				offset = strlen(s);
			}
			sprintf(s + offset, ")");
			break;
		}
		default:
			break;
	}
	return s;
}

static struct ast_node expression_binding_power(
    struct lexer *lexer,
    uint8_t min_bp
) {
	struct token token;

	struct binding_power bp;
	struct ast_node lhs = {};
	token = lexer_next(lexer);
	if (token.category == TOKEN_CATEGORY_LITERAL) {
		lhs.category = AST_CATEGORY_LITERAL;
		switch (token.type) {
			case TOKEN_NUMBER:
				lhs.type = AST_TYPE_INTEGER_LITERAL;
				char *s = lexeme_to_string(token.lexeme);
				lhs.literal.integer = atoi(s);
				free(s);
				break;
			case TOKEN_IDENTIFIER:
				lhs.type = AST_TYPE_IDENTIFIER;
				lhs.literal.identifier = lexeme_to_string(token.lexeme);
				break;
			default:
				fprintf(
				    stderr, "bad literal type %s\n",
				    token_type_to_string(token.type)
				);
				exit(1);
		}
	} else if (token.type == TOKEN_LEFT_PAREN) {
		lhs = expression_binding_power(lexer, 0);
		token = lexer_next(lexer);
		if (token.type != TOKEN_RIGHT_PAREN) {
			fprintf(stderr, "expected closing parenthesis\n");
			exit(1);
		}
	} else if ((bp = prefix_binding_power(token.type)).some) {
		struct ast_node rhs = expression_binding_power(lexer, bp.right);

		lhs.type = get_ast_node_type(token.type);
		lhs.category = AST_CATEGORY_OPERAND;
		operand_add_child(&lhs.operand, rhs);
	} else {
		fprintf(
		    stderr, "bad token %d: '%s', expected atom\n", token.type,
		    lexeme_to_string(token.lexeme)
		);
		exit(1);
	}

	for (;;) {
		token = lexer_peek(lexer);
		if (token.type == TOKEN_EOF) {
			break;
		}
		enum token_type op;
		switch (token.category) {
			case TOKEN_CATEGORY_OPERATOR:
				op = token.type;
				break;
			default:
				fprintf(
				    stderr, "bad token %d: '%s', expected operator\n",
				    token.type, lexeme_to_string(token.lexeme)
				);
				exit(1);
		}

		struct binding_power bp;
		bp = postfix_binding_power(op);
		if (bp.some) {
			if (bp.left < min_bp) {
				break;
			}
			lexer_next(lexer);

			struct ast_node expr = {};
			expr.type = get_ast_node_type(op);
			expr.category = AST_CATEGORY_OPERAND;
			operand_add_child(&expr.operand, lhs);

			if (op == TOKEN_LEFT_BRACKET) {
				struct ast_node rhs = expression_binding_power(lexer, 0);
				token = lexer_next(lexer);
				if (token.type != TOKEN_RIGHT_BRACKET) {
					fprintf(stderr, "expected closing bracket\n");
					exit(1);
				}
				operand_add_child(&expr.operand, rhs);
			} else if (op == TOKEN_LEFT_PAREN) {
				size_t parameter_count = 0;

				while ((token = lexer_peek(lexer)).type != TOKEN_RIGHT_PAREN) {
					if (parameter_count != 0) {
						if (token.type != TOKEN_COMMA) {
							fprintf(stderr, "expected comma\n");
							exit(1);
						}
						lexer_next(lexer);
					}
					operand_add_child(
					    &expr.operand, expression_binding_power(lexer, 0)
					);
					parameter_count++;
				}

				lexer_next(lexer);
			}

			lhs = expr;
			continue;
		}

		bp = infix_binding_power(op);
		if (bp.some) {
			if (bp.left < min_bp) {
				break;
			}

			lexer_next(lexer);

			struct ast_node expr = {};
			expr.type = get_ast_node_type(op);
			expr.category = AST_CATEGORY_OPERAND;

			if (op == TOKEN_QUESTION) {
				struct ast_node mhs = expression_binding_power(lexer, 0);

				token = lexer_next(lexer);
				if (token.type != TOKEN_COLON) {
					fprintf(stderr, "expected colon\n");
					exit(1);
				}

				struct ast_node rhs = expression_binding_power(lexer, bp.right);

				operand_add_child(&expr.operand, lhs);
				operand_add_child(&expr.operand, mhs);
				operand_add_child(&expr.operand, rhs);
			} else {
				struct ast_node rhs = expression_binding_power(lexer, bp.right);

				operand_add_child(&expr.operand, lhs);
				operand_add_child(&expr.operand, rhs);
			}

			lhs = expr;
			continue;
		}

		break;
	}

	return lhs;
}

static struct binding_power prefix_binding_power(enum token_type op) {
	switch (op) {
		case TOKEN_BANG:
		case TOKEN_PLUS:
		case TOKEN_DASH:
			return (struct binding_power){0, 5, true};
		default:
			fprintf(stderr, "bad op %s\n", token_type_to_string(op));
			exit(1);
	}
}

static struct binding_power infix_binding_power(enum token_type op) {
	switch (op) {
		case TOKEN_EQUALS:
			return (struct binding_power){2, 1, true};
		case TOKEN_QUESTION:
			return (struct binding_power){4, 3, true};
		case TOKEN_PLUS:
		case TOKEN_DASH:
			return (struct binding_power){1, 2, true};
		case TOKEN_STAR:
		case TOKEN_SLASH:
			return (struct binding_power){3, 4, true};
		case TOKEN_DOT:
			return (struct binding_power){10, 9, true};
		default:
			return (struct binding_power){.some = false};
	}
}

static struct binding_power postfix_binding_power(enum token_type op) {
	switch (op) {
		case TOKEN_LEFT_PAREN:
		case TOKEN_LEFT_BRACKET:
			return (struct binding_power){7, 0, true};
		default:
			return (struct binding_power){.some = false};
	}
}

static enum ast_node_type get_ast_node_type(enum token_type op) {
	switch (op) {
		case TOKEN_PLUS:
			return AST_TYPE_ADD;
		case TOKEN_DASH:
			return AST_TYPE_SUBTRACT;
		case TOKEN_STAR:
			return AST_TYPE_MULTIPLY;
		case TOKEN_SLASH:
			return AST_TYPE_DIVIDE;
		case TOKEN_LEFT_PAREN:
			return AST_TYPE_FUNCTION_CALL;
		case TOKEN_LEFT_BRACKET:
			return AST_TYPE_INDEX;
		case TOKEN_DOT:
			return AST_TYPE_MEMBER;
		case TOKEN_QUESTION:
			return AST_TYPE_TERNARY;
		case TOKEN_EQUALS:
			return AST_TYPE_ASSIGN;
		default:
			fprintf(stderr, "bad op %s\n", token_type_to_string(op));
			exit(1);
	}
}
