#include "gen.h"

#include "ast.h"
#include "codegen.h"

#include <stdlib.h>

static int genAST(struct ast_node n, FILE *out_file);

void generatecode(struct ast_node n, FILE *out_file) {
	int reg;

	cgpreamble(out_file);
	reg = genAST(n, out_file);
	cgprintint(reg, out_file);
	cgpostamble(out_file);
}

static int genAST(struct ast_node n, FILE *out_file) {
	switch (n.category) {
		case AST_CATEGORY_LITERAL:
			return cgload(n.literal.integer, out_file);
		case AST_CATEGORY_OPERAND: {
			int subregisters[n.operand.child_count];
			for (size_t i = 0; i < n.operand.child_count; i++) {
				subregisters[i] = genAST(n.operand.children[i], out_file);
			}

			switch (n.type) {
				case AST_TYPE_ADD:
					return cgadd(subregisters[0], subregisters[1], out_file);
				case AST_TYPE_SUBTRACT:
					return cgsub(subregisters[0], subregisters[1], out_file);
				case AST_TYPE_MULTIPLY:
					return cgmul(subregisters[0], subregisters[1], out_file);
				case AST_TYPE_DIVIDE:
					return cgdiv(subregisters[0], subregisters[1], out_file);
				default:
					fprintf(stderr, "Unsupported operator: %d\n", n.type);
					exit(1);
			}
		}
		default:
			fprintf(stderr, "Unsupported expression type: %d\n", n.type);
			exit(1);
	}
}
