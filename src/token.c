#include "token.h"

#include <stdlib.h>
#include <string.h>

char *lexeme_to_string(struct lexeme lexeme) {
	char *str = malloc(lexeme.length + 1);
	memcpy(str, lexeme.start, lexeme.length);
	str[lexeme.length] = '\0';
	return str;
}
