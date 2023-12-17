#ifndef _TOKEN_H
#define _TOKEN_H

enum token_type {
	TOKEN_EOF,
	TOKEN_ATOM,
	TOKEN_OP,
};

struct token {
	enum token_type type;
	char lexeme;
};

#endif // _TOKEN_H
