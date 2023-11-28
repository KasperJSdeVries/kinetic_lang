#ifndef _KINETIC_LANG_TOKEN_H
#define _KINETIC_LANG_TOKEN_H

#include <assert/assert.h>

// std
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define ENUMERATE_TOKEN_TYPES                                                  \
	__ENUMERATE_TOKEN_TYPE(INVALID)                                            \
	__ENUMERATE_TOKEN_TYPE(LEFT_PAREN)                                         \
	__ENUMERATE_TOKEN_TYPE(RIGHT_PAREN)                                        \
	__ENUMERATE_TOKEN_TYPE(LEFT_BRACE)                                         \
	__ENUMERATE_TOKEN_TYPE(RIGHT_BRACE)                                        \
	__ENUMERATE_TOKEN_TYPE(LEFT_BRACKET)                                       \
	__ENUMERATE_TOKEN_TYPE(RIGHT_BRACKET)                                      \
	__ENUMERATE_TOKEN_TYPE(COLON)                                              \
	__ENUMERATE_TOKEN_TYPE(SEMICOLON)                                          \
	__ENUMERATE_TOKEN_TYPE(DOT)                                                \
	__ENUMERATE_TOKEN_TYPE(COMMA)                                              \
	__ENUMERATE_TOKEN_TYPE(UNDERSCORE)                                         \
	__ENUMERATE_TOKEN_TYPE(EOF)

typedef enum {
#define __ENUMERATE_TOKEN_TYPE(x) TOKEN_##x,
	ENUMERATE_TOKEN_TYPES
#undef __ENUMERATE_TOKEN_TYPE
} TokenType;

static char *token_type_as_string(TokenType type) {
	switch (type) {
#define __ENUMERATE_TOKEN_TYPE(x)                                              \
	case TOKEN_##x:                                                            \
		return #x;
		ENUMERATE_TOKEN_TYPES
#undef __ENUMERATE_TOKEN_TYPE
		default:
			ASSERT_NOT_REACHED();
	}
}

typedef struct {
	size_t line;
	size_t column;
} Position;

static Position position_create(size_t line, size_t column) {
	return (Position){
	    .line = line,
	    .column = column,
	};
}

typedef struct {
	const char *ptr;
	size_t length;
} Lexeme;

static Lexeme lexeme_create(
    const char *input,
    size_t start_offset,
    size_t length
) {
	return (Lexeme){
	    .ptr = input + start_offset,
	    .length = length,
	};
}

typedef struct {
	TokenType type;
	Lexeme lexeme;
	Position start;
	Position end;
	union {
		union {
			char character;
			char *string;

			uint8_t u8;
			uint16_t u16;
			uint32_t u32;
			uint64_t u64;

			int8_t i8;
			int16_t i16;
			int32_t i32;
			int64_t i64;

			bool b8;

			float f32;
			double f64;
		} literal;

		// NOTE: Can add things like symbol table entry data
		char *identifier;
	} data;
} Token;

static Token token_create(
    TokenType type,
    Lexeme lexeme,
    Position start,
    Position end
) {
	Token token = {
	    .type = type,
	    .lexeme = lexeme,
	    .start = start,
	    .end = end,
	    .data = {},
	};
	return token;
}

#undef ENUMERATE_TOKEN_TYPES

#endif // _KINETIC_LANG_TOKEN_H
