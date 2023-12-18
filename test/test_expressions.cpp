#include "gtest/gtest.h"

extern "C" {
#include "expression.h"
}

TEST(expressions, test_single_char) {
	struct expression expr = expression_new("1");
	ASSERT_STREQ(expression_format(&expr), "1");
}

TEST(expressions, test_simple_expression) {
	struct expression expr = expression_new("1 + 2 * 3");
	ASSERT_STREQ(expression_format(&expr), "(+ 1 (* 2 3))");
}

TEST(expressions, test_binding_power) {
	struct expression expr = expression_new("a + b * c * d + e");
	ASSERT_STREQ(expression_format(&expr), "(+ (+ a (* (* b c) d)) e)");
}

TEST(expressions, test_points) {
	struct expression expr = expression_new("f . g . h");
	ASSERT_STREQ(expression_format(&expr), "(. f (. g h))");
}

TEST(expressions, test_points_and_bp) {
	struct expression expr = expression_new(" 1 + 2 + f . g . h * 3 * 4");
	ASSERT_STREQ(
	    expression_format(&expr), "(+ (+ 1 2) (* (* (. f (. g h)) 3) 4))"
	);
}

TEST(expressions, test_prefix_ops) {
	struct expression expr = expression_new("--1 * 2");
	ASSERT_STREQ(expression_format(&expr), "(* (- (- 1)) 2)");
}

TEST(expressions, test_prefix_op_and_points) {
	struct expression expr = expression_new("--f . g");
	ASSERT_STREQ(expression_format(&expr), "(- (- (. f g)))");
}

TEST(expressions, test_prefix_and_postfix_ops) {
	struct expression expr = expression_new("-9!");
	ASSERT_STREQ(expression_format(&expr), "(- (! 9))");
}

TEST(expressions, test_postfix_and_dot_ops) {
	struct expression expr = expression_new("f . g !");
	ASSERT_STREQ(expression_format(&expr), "(! (. f g))");
}

TEST(expressions, test_parens) {
	struct expression expr = expression_new("(((0)))");
	ASSERT_STREQ(expression_format(&expr), "0");
}

TEST(expressions, test_brackets) {
	struct expression expr = expression_new("x[0][1]");
	ASSERT_STREQ(expression_format(&expr), "([ ([ x 0) 1)");
}

TEST(expressions, test_ternaries) {
	struct expression expr = expression_new("a ? b :"
	                                        "c ? d"
	                                        ": e");
	ASSERT_STREQ(expression_format(&expr), "(? a b (? c d e))");
}

TEST(expressions, test_ternaries_and_equals) {
	struct expression expr = expression_new("a = 0 ? b : c = d");
	ASSERT_STREQ(expression_format(&expr), "(= a (= (? 0 b c) d))");
}

TEST(expressions, test_function_calls) {
	struct expression expr = expression_new("bruh(a, b, c)");
	ASSERT_STREQ(expression_format(&expr), "(( bruh a b c)");
}

TEST(expressions, test_nested_function_calls) {
	struct expression expr = expression_new("bruh(a, b(d, e(f)), c)");
	ASSERT_STREQ(expression_format(&expr), "(( bruh a (( b d (( e f)) c)");
}
