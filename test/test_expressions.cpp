#include "gtest/gtest.h"

extern "C" {
#include "ast.h"
#include "expression.h"
}

TEST(expressions, test_single_char) {
	struct ast_node expr = expression_parse("1");
	char *s = expression_format(&expr);
	ASSERT_STREQ(s, "1");
	free(s);
	ast_node_free(expr);
}

TEST(expressions, test_simple_expression) {
	struct ast_node expr = expression_parse("1 + 2 * 3");
	char *s = expression_format(&expr);
	ASSERT_STREQ(s, "(+ 1 (* 2 3))");
	free(s);
	ast_node_free(expr);
}

TEST(expressions, test_binding_power) {
	struct ast_node expr = expression_parse("a + b * c * d + e");
	char *s = expression_format(&expr);
	ASSERT_STREQ(s, "(+ (+ a (* (* b c) d)) e)");
	free(s);
	ast_node_free(expr);
}

TEST(expressions, test_points) {
	struct ast_node expr = expression_parse("f . g . h");
	char *s = expression_format(&expr);
	ASSERT_STREQ(s, "(. f (. g h))");
	free(s);
	ast_node_free(expr);
}

TEST(expressions, test_points_and_bp) {
	struct ast_node expr = expression_parse(" 1 + 2 + f . g . h * 3 * 4");
	char *s = expression_format(&expr);
	ASSERT_STREQ(s, "(+ (+ 1 2) (* (* (. f (. g h)) 3) 4))");
	free(s);
	ast_node_free(expr);
}

TEST(expressions, test_prefix_ops) {
	struct ast_node expr = expression_parse("--1 * 2");
	char *s = expression_format(&expr);
	ASSERT_STREQ(s, "(* (- (- 1)) 2)");
	free(s);
	ast_node_free(expr);
}

TEST(expressions, test_prefix_op_and_points) {
	struct ast_node expr = expression_parse("--f . g");
	char *s = expression_format(&expr);
	ASSERT_STREQ(s, "(- (- (. f g)))");
	free(s);
	ast_node_free(expr);
}

TEST(expressions, test_parens) {
	struct ast_node expr = expression_parse("(((0)))");
	char *s = expression_format(&expr);
	ASSERT_STREQ(s, "0");
	free(s);
	ast_node_free(expr);
}

TEST(expressions, test_brackets) {
	struct ast_node expr = expression_parse("x[0][1]");
	char *s = expression_format(&expr);
	ASSERT_STREQ(s, "([] ([] x 0) 1)");
	free(s);
	ast_node_free(expr);
}

TEST(expressions, test_ternaries) {
	struct ast_node expr = expression_parse("a ? b :"
	                                        "c ? d"
	                                        ": e");
	char *s = expression_format(&expr);
	ASSERT_STREQ(s, "(?: a b (?: c d e))");
	free(s);
	ast_node_free(expr);
}

TEST(expressions, test_ternaries_and_equals) {
	struct ast_node expr = expression_parse("a = 0 ? b : c = d");
	char *s = expression_format(&expr);
	ASSERT_STREQ(s, "(= a (= (?: 0 b c) d))");
	free(s);
	ast_node_free(expr);
}

TEST(expressions, test_function_calls) {
	struct ast_node expr = expression_parse("bruh(a, b, c)");
	char *s = expression_format(&expr);
	ASSERT_STREQ(s, "(() bruh a b c)");
	free(s);
	ast_node_free(expr);
}

TEST(expressions, test_nested_function_calls) {
	struct ast_node expr = expression_parse("bruh(a, b(d, e(f)), c)");
	char *s = expression_format(&expr);
	ASSERT_STREQ(s, "(() bruh a (() b d (() e f)) c)");
	free(s);
	ast_node_free(expr);
}
