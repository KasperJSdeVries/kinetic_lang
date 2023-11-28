#ifndef _KINETIC_LANG_ASSERT_H
#define _KINETIC_LANG_ASSERT_H

#include <stdio.h>
#include <stdlib.h>

__attribute__((noreturn)) static void assertion_failed(
    const char *assertion,
    const char *file,
    int line,
    const char *function
) {
	printf("%s at %s:%d in %s\n", assertion, file, line, function);
	exit(EXIT_FAILURE);
}

#define ASSERT(expr)                                                           \
	((!(expr))                                                                 \
	     ? assertion_failed(#expr, __FILE__, __LINE__, __PRETTY_FUNCTION__)    \
	     : (void)0)

#define ASSERT_NOT_REACHED() ASSERT(false)

#define TODO()                                                                 \
	assertion_failed("TODO", __FILE__, __LINE__, __PRETTY_FUNCTION__);

#endif // _KINETIC_LANG_ASSERT_H
