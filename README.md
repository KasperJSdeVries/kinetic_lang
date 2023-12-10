
# Grammar

```bison
module
    : declaration_list
    ;

declaration_list
    : declaration
    | declaration declaration_list
    ;

declaration
    : function_declaration
    | variable_declaration
    | struct_declaration
    | interface_declaration
    | imlementation_declaration
    ;

function_declaration
    : type identifier '(' parameter_list ')' block_statement
    ;

variable_declaration
    : type identifier ';'
    | type identifier '=' expression ';'
    ;

struct_declaration
    : 'struct' identifier '{' struct_member_list '}'
    ;

struct_member_list
    : struct_member
    | struct_member struct_member_list
    ;

struct_member
    : type identifier ';'
    ;

interface_declaration
    : 'interface' identifier '{' interface_member_list '}'
    ;

interface_member_list
    : interface_member
    | interface_member interface_member_list
    ;

interface_member
    : type identifier '(' parameter_list ')' ';'
    ;

imlementation_declaration
    : 'implement' identifier '{' implementation_member_list '}'
    | 'implement' identifier 'for' identifier '{' implementation_member_list '}'
    ;

implementation_member_list
    : implementation_member
    | implementation_member implementation_member_list
    ;

implementation_member
    : type identifier '(' parameter_list ')' block_statement
    | type identifier '(' this_parameter ',' parameter_list ')' block_statement
    | type identifier '(' this_parameter ')' block_statement
    ;
```
