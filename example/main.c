#include "ast.h"
#include "expression.h"
#include "gen.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: %s <in_file>\n", argv[0]);
		return 1;
	}

	FILE *in_file = fopen(argv[1], "r");
	if (in_file == NULL) {
		fprintf(
		    stderr, "Error: Unable to open %s: %s\n", argv[1], strerror(errno)
		);
		exit(1);
	}

	// Read file to buffer
	fseek(in_file, 0, SEEK_END);
	int size = ftell(in_file);
	fseek(in_file, 0, SEEK_SET);
	char buffer[size + 1];
	buffer[size] = '\0';
	fread(buffer, 1, size, in_file);
	fclose(in_file);

	FILE *out_file = fopen("out.s", "w");
	if (out_file == NULL) {
		fprintf(stderr, "Error: Unable to open out.s: %s\n", strerror(errno));
		exit(1);
	}

	struct ast_node expression = expression_parse(buffer);
	generatecode(expression, out_file);

	ast_node_free(expression);

	fclose(out_file);

	return 0;
}
