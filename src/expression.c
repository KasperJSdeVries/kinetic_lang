#include "expression.h"
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

static struct expression expression_binding_power(
    struct lexer *lexer,
    uint8_t min_bp
);
static struct binding_power prefix_binding_power(char op);
static struct binding_power infix_binding_power(char op);
static struct binding_power postfix_binding_power(char op);

struct expression expression_new(char *input) {
	struct lexer lexer = lexer_new(input);
	return expression_binding_power(&lexer, 0);
}

void expression_free(struct expression *expr) {
	if (expr->type == E_CONS) {
		for (int i = 0; i < expr->cons.subexpr_count; i++) {
			expression_free(&expr->cons.subexprs[i]);
		}
		free(expr->cons.subexprs);
	}
}

char *expression_format(const struct expression *expr) {
	char *s = calloc(1024, 1);
	size_t offset = 0;
	switch (expr->type) {
		case E_ATOM:
			sprintf(s + offset, "%c", expr->atom.id);
			break;
		case E_CONS:
			sprintf(s + offset, "(%c", expr->cons.op);
			offset = strlen(s);
			for (int i = 0; i < expr->cons.subexpr_count; i++) {
				sprintf(
				    s + offset, " %s",
				    expression_format(&expr->cons.subexprs[i])
				);
				offset = strlen(s);
			}
			sprintf(s + offset, ")");
			break;
	}
	return s;
}

static struct expression expression_binding_power(
    struct lexer *lexer,
    uint8_t min_bp
) {
	struct token token;

	struct expression lhs;
	token = lexer_next(lexer);
	switch (token.type) {
		case TOKEN_ATOM:
			lhs.type = E_ATOM;
			lhs.atom.id = token.lexeme;
			break;

		case TOKEN_OP: {
			if (token.lexeme == '(') {
				lhs = expression_binding_power(lexer, 0);
				token = lexer_next(lexer);
				if (token.type != TOKEN_OP || token.lexeme != ')') {
					fprintf(stderr, "expected closing parenthesis\n");
					exit(1);
				}
			} else {
				struct binding_power bp = prefix_binding_power(token.lexeme);
				struct expression rhs =
				    expression_binding_power(lexer, bp.right);

				lhs.type = E_CONS;
				lhs.cons.op = token.lexeme;
				lhs.cons.subexpr_count = 1;
				lhs.cons.subexprs = calloc(1, sizeof(struct expression));
				lhs.cons.subexprs[0] = rhs;
			}

			break;
		}
		default:
			fprintf(
			    stderr, "bad token %d: '%c', expected atom\n", token.type,
			    token.lexeme
			);
			exit(1);
	}

	for (;;) {
		token = lexer_peek(lexer);
		if (token.type == TOKEN_EOF) {
			break;
		}
		char op;
		switch (token.type) {
			case TOKEN_OP:
				op = token.lexeme;
				break;
			default:
				fprintf(
				    stderr, "bad token %d: '%c', expected operator\n",
				    token.type, token.lexeme
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

			struct expression expr;
			expr.type = E_CONS;
			expr.cons.op = op;
			if (op == '[') {
				struct expression rhs = expression_binding_power(lexer, 0);
				token = lexer_next(lexer);
				if (token.type != TOKEN_OP || token.lexeme != ']') {
					fprintf(stderr, "expected closing bracket\n");
					exit(1);
				}
				expr.cons.subexpr_count = 2;
				expr.cons.subexprs = calloc(2, sizeof(struct expression));
				expr.cons.subexprs[0] = lhs;
				expr.cons.subexprs[1] = rhs;

			} else {
				expr.cons.subexpr_count = 1;
				expr.cons.subexprs = calloc(1, sizeof(struct expression));
				expr.cons.subexprs[0] = lhs;
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

			struct expression expr;
			expr.type = E_CONS;
			expr.cons.op = op;

			if (op == '?') {
				struct expression mhs = expression_binding_power(lexer, 0);

				token = lexer_next(lexer);
				if (token.type != TOKEN_OP || token.lexeme != ':') {
					fprintf(stderr, "expected colon\n");
					exit(1);
				}

				struct expression rhs =
				    expression_binding_power(lexer, bp.right);

				expr.cons.subexpr_count = 3;
				expr.cons.subexprs = calloc(3, sizeof(struct expression));
				expr.cons.subexprs[0] = lhs;
				expr.cons.subexprs[1] = mhs;
				expr.cons.subexprs[2] = rhs;

			} else {
				struct expression rhs =
				    expression_binding_power(lexer, bp.right);

				expr.cons.subexpr_count = 2;
				expr.cons.subexprs = calloc(2, sizeof(struct expression));
				expr.cons.subexprs[0] = lhs;
				expr.cons.subexprs[1] = rhs;
			}

			lhs = expr;
			continue;
		}

		break;
	}

	return lhs;
}

static struct binding_power prefix_binding_power(char op) {
	switch (op) {
		case '+':
		case '-':
			return (struct binding_power){0, 5, true};
		default:
			fprintf(stderr, "bad op %c\n", op);
			exit(1);
	}
}

static struct binding_power infix_binding_power(char op) {
	switch (op) {
		case '=':
			return (struct binding_power){2, 1, true};
		case '?':
			return (struct binding_power){4, 3, true};
		case '+':
		case '-':
			return (struct binding_power){1, 2, true};
		case '*':
		case '/':
			return (struct binding_power){3, 4, true};
		case '.':
			return (struct binding_power){10, 9, true};
		default:
			return (struct binding_power){.some = false};
	}
}

static struct binding_power postfix_binding_power(char op) {
	switch (op) {
		case '!':
		case '[':
			return (struct binding_power){7, 0, true};
		default:
			return (struct binding_power){.some = false};
	}
}
