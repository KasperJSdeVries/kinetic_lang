#include <parser/parser.h>

#include <stddef.h>

struct Parser_t {
	Token *tokens;
};

Parser *parser_create(Token *tokens) {
	if (tokens == NULL) {
		return NULL;
	}

	Parser *parser = malloc(sizeof(struct Parser_t));
	parser->tokens = tokens;

	return parser;
}

void parser_free(Parser *parser) {
	free(parser);
}

Module *parser_parse(Parser *parser) {
	Module *module = malloc(sizeof(Module));

	return module;
}
