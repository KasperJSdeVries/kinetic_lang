#include <lexer/token.h>
#include <stdio.h>

int main(void) {
	Token t = {0};
	printf("Token.type: %lu\n", sizeof(t.type));
	printf("Token.lexeme: %lu\n", sizeof(t.lexeme));
	printf("Token.start: %lu\n", sizeof(t.start));
	printf("Token.end: %lu\n", sizeof(t.end));
	printf("Token.data: %lu\n", sizeof(t.data));
	return 0;
}
