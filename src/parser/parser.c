#include <lexer/token.h>
#include <parser/ast.h>
#include <parser/parser.h>

#include <stddef.h>

struct Parser_t {
	Token *tokens;
	size_t current_index;
};

Token parser_get_current_token(const Parser *parser);

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

Module *parser_parse_module(Parser *parser, const char *name) {
	Module *module = malloc(sizeof(Module));

	module->name = (Identifier){
	    .name = name,
	};

	while (parser_get_current_token(parser).type != TOKEN_EOF) {
		// TODO: Populate this
	}

	return module;
}

Token parser_get_current_token(const Parser *parser) {
	return parser->tokens[parser->current_index];
}
