#include "expression.h"
#include "lexer.h"
#include "token.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define ADD_TEST(test_name)                                                    \
	do {                                                                       \
		printf("%s: ", #test_name);                                            \
		fflush(stdout);                                                        \
		test_name() ? printf("PASS\n") : printf("FAILED\n");                   \
	} while (0)

#define TEST(name) bool(name)()

#define START_TEST bool test_result = true;
#define END_TEST return test_result;

#define ASSERT_EQ(a, b)                                                        \
	if (a != b) {                                                              \
		test_result = false;                                                   \
	}
#define ASSERT_STREQ(a, b)                                                     \
	if (strcmp(a, b) != 0) {                                                   \
		test_result = false;                                                   \
	}

TEST(test_lexer) {
	START_TEST;
	struct lexer lexer = lexer_new("1 + 2 * 3 !");
	struct token token;

	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_ATOM);
	ASSERT_EQ(token.lexeme, '1');
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_OP);
	ASSERT_EQ(token.lexeme, '+');
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_ATOM);
	ASSERT_EQ(token.lexeme, '2');
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_OP);
	ASSERT_EQ(token.lexeme, '*');
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_ATOM);
	ASSERT_EQ(token.lexeme, '3');
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_OP);
	ASSERT_EQ(token.lexeme, '!');
	token = lexer_next(&lexer);
	ASSERT_EQ(token.type, TOKEN_EOF);

	END_TEST;
}

TEST(test_single_char) {
	START_TEST;
	struct expression expr = expression_new("1");
	ASSERT_STREQ(expression_format(&expr), "1");
	END_TEST;
}

TEST(test_simple_expression) {
	START_TEST;
	struct expression expr = expression_new("1 + 2 * 3");
	ASSERT_STREQ(expression_format(&expr), "(+ 1 (* 2 3))");
	END_TEST;
}

TEST(test_binding_power) {
	START_TEST;
	struct expression expr = expression_new("a + b * c * d + e");
	ASSERT_STREQ(expression_format(&expr), "(+ (+ a (* (* b c) d)) e)");
	END_TEST;
}

TEST(test_points) {
	START_TEST;
	struct expression expr = expression_new("f . g . h");
	ASSERT_STREQ(expression_format(&expr), "(. f (. g h))");
	END_TEST;
}

TEST(test_points_and_bp) {
	START_TEST;
	struct expression expr = expression_new(" 1 + 2 + f . g . h * 3 * 4");
	ASSERT_STREQ(
	    expression_format(&expr), "(+ (+ 1 2) (* (* (. f (. g h)) 3) 4))"
	);
	END_TEST;
}

TEST(test_prefix_ops) {
	START_TEST;
	struct expression expr = expression_new("--1 * 2");
	ASSERT_STREQ(expression_format(&expr), "(* (- (- 1)) 2)");
	END_TEST;
}

TEST(test_prefix_op_and_points) {
	START_TEST;
	struct expression expr = expression_new("--f . g");
	ASSERT_STREQ(expression_format(&expr), "(- (- (. f g)))");
	END_TEST;
}

TEST(test_prefix_and_postfix_ops) {
	START_TEST;
	struct expression expr = expression_new("-9!");
	ASSERT_STREQ(expression_format(&expr), "(- (! 9))");
	END_TEST;
}

TEST(test_postfix_and_dot_ops) {
	START_TEST;
	struct expression expr = expression_new("f . g !");
	ASSERT_STREQ(expression_format(&expr), "(! (. f g))");
	END_TEST;
}

TEST(test_parens) {
	START_TEST;
	struct expression expr = expression_new("(((0)))");
	ASSERT_STREQ(expression_format(&expr), "0");
	END_TEST;
}

TEST(test_brackets) {
	START_TEST;
	struct expression expr = expression_new("x[0][1]");
	ASSERT_STREQ(expression_format(&expr), "([ ([ x 0) 1)");
	END_TEST;
}

TEST(test_ternaries) {
	START_TEST;
	struct expression expr = expression_new("a ? b :"
	                                        "c ? d"
	                                        ": e");
	ASSERT_STREQ(expression_format(&expr), "(? a b (? c d e))");
	END_TEST;
}

TEST(test_ternaries_and_equals) {
	START_TEST;
	struct expression expr = expression_new("a = 0 ? b : c = d");
	ASSERT_STREQ(expression_format(&expr), "(= a (= (? 0 b c) d))");
	END_TEST;
}

int main(int argc, char *argv[]) {
	ADD_TEST(test_lexer);
	ADD_TEST(test_single_char);
	ADD_TEST(test_simple_expression);
	ADD_TEST(test_binding_power);
	ADD_TEST(test_points);
	ADD_TEST(test_points_and_bp);
	ADD_TEST(test_prefix_ops);
	ADD_TEST(test_prefix_op_and_points);
	ADD_TEST(test_prefix_and_postfix_ops);
	ADD_TEST(test_postfix_and_dot_ops);
	ADD_TEST(test_parens);
	ADD_TEST(test_brackets);
	ADD_TEST(test_ternaries);
	ADD_TEST(test_ternaries_and_equals);
}
