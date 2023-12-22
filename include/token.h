#ifndef _TOKEN_H
#define _TOKEN_H

#include <stddef.h>

#define _ENUMERATE_SYMBOLS                                                     \
	_ENUMERATE_TOKEN(TOKEN_LEFT_PAREN, "(")                                    \
	_ENUMERATE_TOKEN(TOKEN_RIGHT_PAREN, ")")                                   \
	_ENUMERATE_TOKEN(TOKEN_LEFT_BRACKET, "[")                                  \
	_ENUMERATE_TOKEN(TOKEN_RIGHT_BRACKET, "]")                                 \
	_ENUMERATE_TOKEN(TOKEN_LEFT_BRACE, "{")                                    \
	_ENUMERATE_TOKEN(TOKEN_RIGHT_BRACE, "}")                                   \
	_ENUMERATE_TOKEN(TOKEN_ARROW, "->")                                        \
	_ENUMERATE_TOKEN(TOKEN_PLUS_PLUS, "++")                                    \
	_ENUMERATE_TOKEN(TOKEN_DASH_DASH, "--")                                    \
	_ENUMERATE_TOKEN(TOKEN_DOT, ".")                                           \
	_ENUMERATE_TOKEN(TOKEN_BANG, "!")                                          \
	_ENUMERATE_TOKEN(TOKEN_TILDE, "~")                                         \
	_ENUMERATE_TOKEN(TOKEN_STAR, "*")                                          \
	_ENUMERATE_TOKEN(TOKEN_SLASH, "/")                                         \
	_ENUMERATE_TOKEN(TOKEN_PERCENT, "%")                                       \
	_ENUMERATE_TOKEN(TOKEN_PLUS, "+")                                          \
	_ENUMERATE_TOKEN(TOKEN_DASH, "-")                                          \
	_ENUMERATE_TOKEN(TOKEN_LESS_LESS, "<<")                                    \
	_ENUMERATE_TOKEN(TOKEN_GREATER_GREATER, ">>")                              \
	_ENUMERATE_TOKEN(TOKEN_LESS, "<")                                          \
	_ENUMERATE_TOKEN(TOKEN_LESS_EQUALS, "<=")                                  \
	_ENUMERATE_TOKEN(TOKEN_GREATER, ">")                                       \
	_ENUMERATE_TOKEN(TOKEN_GREATER_EQUALS, ">=")                               \
	_ENUMERATE_TOKEN(TOKEN_EQUALS_EQUALS, "==")                                \
	_ENUMERATE_TOKEN(TOKEN_BANG_EQUALS, "!=")                                  \
	_ENUMERATE_TOKEN(TOKEN_AMPERSAND, "&")                                     \
	_ENUMERATE_TOKEN(TOKEN_CARET, "^")                                         \
	_ENUMERATE_TOKEN(TOKEN_PIPE, "|")                                          \
	_ENUMERATE_TOKEN(TOKEN_AMPERSAND_AMPERSAND, "&&")                          \
	_ENUMERATE_TOKEN(TOKEN_PIPE_PIPE, "||")                                    \
	_ENUMERATE_TOKEN(TOKEN_QUESTION, "?")                                      \
	_ENUMERATE_TOKEN(TOKEN_COLON, ":")                                         \
	_ENUMERATE_TOKEN(TOKEN_EQUALS, "=")                                        \
	_ENUMERATE_TOKEN(TOKEN_PLUS_EQUALS, "+=")                                  \
	_ENUMERATE_TOKEN(TOKEN_DASH_EQUALS, "-=")                                  \
	_ENUMERATE_TOKEN(TOKEN_STAR_EQUALS, "*=")                                  \
	_ENUMERATE_TOKEN(TOKEN_SLASH_EQUALS, "/=")                                 \
	_ENUMERATE_TOKEN(TOKEN_PERCENT_EQUALS, "%=")                               \
	_ENUMERATE_TOKEN(TOKEN_LESS_LESS_EQUALS, "<<=")                            \
	_ENUMERATE_TOKEN(TOKEN_GREATER_GREATER_EQUALS, ">>=")                      \
	_ENUMERATE_TOKEN(TOKEN_AMPERSAND_EQUALS, "&=")                             \
	_ENUMERATE_TOKEN(TOKEN_PIPE_EQUALS, "|=")                                  \
	_ENUMERATE_TOKEN(TOKEN_CARET_EQUALS, "^=")                                 \
	_ENUMERATE_TOKEN(TOKEN_COMMA, ",")                                         \
	_ENUMERATE_TOKEN(TOKEN_SEMICOLON, ";")                                     \
	_ENUMERATE_TOKEN(TOKEN_DOUBLE_ARROW, "=>")

#define _ENUMERATE_KEYWORDS                                                    \
	_ENUMERATE_TOKEN(TOKEN_BREAK, "break")                                     \
	_ENUMERATE_TOKEN(TOKEN_CONST, "const")                                     \
	_ENUMERATE_TOKEN(TOKEN_CONTINUE, "continue")                               \
	_ENUMERATE_TOKEN(TOKEN_DO, "do")                                           \
	_ENUMERATE_TOKEN(TOKEN_ELSE, "else")                                       \
	_ENUMERATE_TOKEN(TOKEN_ENUM, "enum")                                       \
	_ENUMERATE_TOKEN(TOKEN_FALSE, "false")                                     \
	_ENUMERATE_TOKEN(TOKEN_FOR, "for")                                         \
	_ENUMERATE_TOKEN(TOKEN_IF, "if")                                           \
	_ENUMERATE_TOKEN(TOKEN_IN, "in")                                           \
	_ENUMERATE_TOKEN(TOKEN_INTERFACE, "interface")                             \
	_ENUMERATE_TOKEN(TOKEN_MUT, "mut")                                         \
	_ENUMERATE_TOKEN(TOKEN_RETURN, "return")                                   \
	_ENUMERATE_TOKEN(TOKEN_STRUCT, "struct")                                   \
	_ENUMERATE_TOKEN(TOKEN_SWITCH, "switch")                                   \
	_ENUMERATE_TOKEN(TOKEN_TRUE, "true")                                       \
	_ENUMERATE_TOKEN(TOKEN_TYPE, "type")                                       \
	_ENUMERATE_TOKEN(TOKEN_UNION, "union")                                     \
	_ENUMERATE_TOKEN(TOKEN_WHILE, "while")

#define _ENUMERATE_LITERAL_TYPES                                               \
	_ENUMERATE_TOKEN(TOKEN_NUMBER, "number")                                   \
	_ENUMERATE_TOKEN(TOKEN_STRING, "string")                                   \
	_ENUMERATE_TOKEN(TOKEN_IDENTIFIER, "identifier")

#define _ENUMERATE_PRIMATIVE_TYPES                                             \
	_ENUMERATE_TOKEN(TOKEN_VOID, "void")                                       \
	_ENUMERATE_TOKEN(TOKEN_BOOL, "bool")                                       \
	_ENUMERATE_TOKEN(TOKEN_CHAR, "char")                                       \
	_ENUMERATE_TOKEN(TOKEN_INT8, "i8")                                         \
	_ENUMERATE_TOKEN(TOKEN_INT16, "i16")                                       \
	_ENUMERATE_TOKEN(TOKEN_INT32, "i32")                                       \
	_ENUMERATE_TOKEN(TOKEN_INT64, "i64")                                       \
	_ENUMERATE_TOKEN(TOKEN_UINT8, "u8")                                        \
	_ENUMERATE_TOKEN(TOKEN_UINT16, "u16")                                      \
	_ENUMERATE_TOKEN(TOKEN_UINT32, "u32")                                      \
	_ENUMERATE_TOKEN(TOKEN_UINT64, "u64")                                      \
	_ENUMERATE_TOKEN(TOKEN_FLOAT32, "f32")                                     \
	_ENUMERATE_TOKEN(TOKEN_FLOAT64, "f64")

#define _ENUMERATE_TOKEN(name, chars) name,
enum token_type {
	TOKEN_EOF,
	// Operators
	_ENUMERATE_SYMBOLS

	    // Literals
	    TOKEN_NUMBER,
	TOKEN_IDENTIFIER,

	// Keywords
	_ENUMERATE_KEYWORDS
};
#undef _ENUMERATE_TOKEN

struct lexeme {
	const char *start;
	size_t length;
};

enum token_category {
	TOKEN_CATEGORY_UNKNOWN,
	TOKEN_CATEGORY_OPERATOR,
	TOKEN_CATEGORY_LITERAL,
};

struct token {
	enum token_type type;
	enum token_category category;
	struct lexeme lexeme;
};

char *token_type_to_string(enum token_type type);
char *lexeme_to_string(struct lexeme lexeme);

#endif // _TOKEN_H
