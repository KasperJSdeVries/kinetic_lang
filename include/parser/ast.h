#ifndef _KINETIC_LANG_AST_H
#define _KINETIC_LANG_AST_H

#include <stddef.h>

typedef struct {
} Identifier;

typedef struct {
} Type;

typedef struct {
	Type type;
	Identifier Name;
} Parameter;

typedef struct {
	Type return_type;
	Identifier name;
	Parameter *parameters;
	size_t parameter_count;
} FunctionDeclaration;

typedef struct {
} VariableDeclaration;

typedef struct {
} StructDeclaration;

typedef struct {
} InterfaceDeclaration;

typedef struct {
} ImplementationDeclaration;

typedef enum {
	DECLARATION_TYPE_FUNCTION,
	DECLARATION_TYPE_VARIABLE,
	DECLARATION_TYPE_STRUCT,
	DECLARATION_TYPE_INTERFACE,
	DECLARATION_TYPE_IMPLEMENTATION,
} DeclarationType;

typedef struct {
	DeclarationType type;
	union {
		FunctionDeclaration function_declaration;
		VariableDeclaration variable_declaration;
		StructDeclaration struct_declaration;
		InterfaceDeclaration interface_declaration;
		ImplementationDeclaration implementation_declaration;
	};
} Declaration;

typedef struct {
	Declaration *declarations;
	size_t declarations_count;

	Identifier name;
} Module;

#endif // _KINETIC_LANG_AST_H
