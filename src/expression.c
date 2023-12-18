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
static struct binding_power prefix_binding_power(enum token_type op);
static struct binding_power infix_binding_power(enum token_type op);
static struct binding_power postfix_binding_power(enum token_type op);

struct expression expression_new(const char *input) {
	struct lexer lexer = lexer_new(input);
	return expression_binding_power(&lexer, 0);
}

void expression_free(struct expression *expr) {
	if (expr->type == E_CONS) {
		for (size_t i = 0; i < expr->cons.subexpr_count; i++) {
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
			sprintf(s + offset, "%s", expr->atom.value);
			break;
		case E_CONS: {
			char *op;
			switch (expr->cons.op) {
				case TOKEN_PLUS:
					op = "+";
					break;
				case TOKEN_DASH:
					op = "-";
					break;
				case TOKEN_STAR:
					op = "*";
					break;
				case TOKEN_SLASH:
					op = "/";
					break;
				case TOKEN_EQUALS:
					op = "=";
					break;
				case TOKEN_LEFT_PAREN:
					op = "(";
					break;
				case TOKEN_RIGHT_PAREN:
					op = ")";
					break;
				case TOKEN_LEFT_BRACKET:
					op = "[";
					break;
				case TOKEN_RIGHT_BRACKET:
					op = "]";
					break;
				case TOKEN_DOT:
					op = ".";
					break;
				case TOKEN_BANG:
					op = "!";
					break;
				case TOKEN_QUESTION:
					op = "?";
					break;
				case TOKEN_COLON:
					op = ":";
					break;
				case TOKEN_SEMICOLON:
					op = ";";
					break;
				default:
					fprintf(stderr, "bad op %d\n", expr->cons.op);
					exit(1);
			}
			sprintf(s + offset, "(%s", op);
			offset = strlen(s);
			for (size_t i = 0; i < expr->cons.subexpr_count; i++) {
				sprintf(
				    s + offset, " %s",
				    expression_format(&expr->cons.subexprs[i])
				);
				offset = strlen(s);
			}
			sprintf(s + offset, ")");
			break;
		}
	}
	return s;
}

static struct expression expression_binding_power(
    struct lexer *lexer,
    uint8_t min_bp
) {
	struct token token;

	struct binding_power bp;
	struct expression lhs;
	token = lexer_next(lexer);
	if (token.category == TOKEN_CATEGORY_LITERAL) {
		lhs.type = E_ATOM;
		lhs.atom.value = lexeme_to_string(token.lexeme);
	} else if (token.type == TOKEN_LEFT_PAREN) {
		lhs = expression_binding_power(lexer, 0);
		token = lexer_next(lexer);
		if (token.type != TOKEN_RIGHT_PAREN) {
			fprintf(stderr, "expected closing parenthesis\n");
			exit(1);
		}
	} else if ((bp = prefix_binding_power(token.type)).some) {
		struct expression rhs = expression_binding_power(lexer, bp.right);

		lhs.type = E_CONS;
		lhs.cons.op = token.type;
		lhs.cons.subexpr_count = 1;
		lhs.cons.subexprs = calloc(1, sizeof(struct expression));
		lhs.cons.subexprs[0] = rhs;
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

			struct expression expr;
			expr.type = E_CONS;
			expr.cons.op = op;
			if (op == TOKEN_LEFT_BRACKET) {
				struct expression rhs = expression_binding_power(lexer, 0);
				token = lexer_next(lexer);
				if (token.type != TOKEN_RIGHT_BRACKET) {
					fprintf(stderr, "expected closing bracket\n");
					exit(1);
				}
				expr.cons.subexpr_count = 2;
				expr.cons.subexprs = calloc(2, sizeof(struct expression));
				expr.cons.subexprs[0] = lhs;
				expr.cons.subexprs[1] = rhs;
			} else if (op == TOKEN_LEFT_PAREN) {
				struct expression *parameters =
				    calloc(1, sizeof(struct expression));
				parameters[0] = lhs;

				size_t parameter_count = 0;

				while ((token = lexer_peek(lexer)).type != TOKEN_RIGHT_PAREN) {
					if (parameter_count != 0) {
						if (token.type != TOKEN_COMMA) {
							fprintf(stderr, "expected comma\n");
							exit(1);
						}
						lexer_next(lexer);
					}
					parameter_count++;
					parameters = reallocarray(
					    parameters, parameter_count + 1,
					    sizeof(struct expression)
					);
					parameters[parameter_count] =
					    expression_binding_power(lexer, 0);
				}

				lexer_next(lexer);

				expr.cons.subexpr_count = parameter_count + 1;
				expr.cons.subexprs = parameters;
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

			if (op == TOKEN_QUESTION) {
				struct expression mhs = expression_binding_power(lexer, 0);

				token = lexer_next(lexer);
				if (token.type != TOKEN_COLON) {
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

static struct binding_power prefix_binding_power(enum token_type op) {
	switch (op) {
		case TOKEN_PLUS:
		case TOKEN_DASH:
			return (struct binding_power){0, 5, true};
		default:
			fprintf(stderr, "bad op %c\n", op);
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
		case TOKEN_BANG:
		case TOKEN_LEFT_PAREN:
		case TOKEN_LEFT_BRACKET:
			return (struct binding_power){7, 0, true};
		default:
			return (struct binding_power){.some = false};
	}
}
