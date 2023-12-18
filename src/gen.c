#include "gen.h"

#include "codegen.h"
#include "expression.h"
#include "token.h"

#include <stdlib.h>

static int genAST(struct expression n, FILE *out_file);

void generatecode(struct expression n, FILE *out_file) {
	int reg;

	cgpreamble(out_file);
	reg = genAST(n, out_file);
	cgprintint(reg, out_file);
	cgpostamble(out_file);
}

static int genAST(struct expression n, FILE *out_file) {
	switch (n.type) {
		case E_ATOM:
			return cgload(atoi(n.atom.value), out_file);
		case E_CONS: {
			int subregisters[n.cons.subexpr_count];
			for (size_t i = 0; i < n.cons.subexpr_count; i++) {
				subregisters[i] = genAST(n.cons.subexprs[i], out_file);
			}

			switch (n.cons.op) {
				case TOKEN_PLUS:
					return cgadd(subregisters[0], subregisters[1], out_file);
				case TOKEN_DASH:
					return cgsub(subregisters[0], subregisters[1], out_file);
				case TOKEN_STAR:
					return cgmul(subregisters[0], subregisters[1], out_file);
				case TOKEN_SLASH:
					return cgdiv(subregisters[0], subregisters[1], out_file);
				default:
					fprintf(stderr, "Unsupported operator: %d\n", n.cons.op);
					exit(1);
			}
		}
		default:
			fprintf(stderr, "Unsupported expression type: %d\n", n.type);
			exit(1);
	}
}
