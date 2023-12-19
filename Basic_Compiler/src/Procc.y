%{
#include "Node.h"
#include <iostream>
#include <string>
extern int yylex();
extern void yyerror(const char* s);

// Global for the program
NProgram* gProgram = nullptr;

// Disable the warnings that Bison creates
#pragma warning(disable: 4065)
%}

/* You should not need to change the union */
%union {
    Node* node;
	NProgram* program;
	NData* data;
	NDecl* decl;
	NBlock* block;
	NStatement* statement;
	NNumeric* numeric;
	NExpr* expr;
	NComparison* comparison;
	std::string* string;
	int token;
}

%error-verbose

/* Tokens/Terminal symbols */
%token <token> TDATA TMAIN TLBRACE TRBRACE TSEMI TLPAREN TRPAREN
%token <token> TLBRACKET TRBRACKET TINC TDEC TEQUALS
%token <token> TADD TSUB TMUL TDIV
%token <token> TLESS TISEQUAL
%token <token> TVAR TARRAY
%token <token> TIF TELSE TWHILE
%token <token> TCOMMA TPENUP TPENDOWN TSETPOS TSETCOLOR TFWD TBACK TROT
%token <string> TINTEGER TIDENTIFIER

/* Types/non-terminal symbols */
%type <program> program
%type <data> data decls
%type <decl> decl;
%type <numeric> numeric
%type <expr> expr
%type <block> main block statements
%type <statement> statement
%type <comparison> comparison

/* Operator precedence */
%left TADD TSUB
%left TMUL TDIV

%%

program		: data main 
				{ 
					std::cout << "Program\n";
					gProgram = new NProgram($1, $2);
				}
;

data		: TDATA TLBRACE TRBRACE
				{
					std::cout << "Data (no decls)\n";
					$$ = new NData();
				}
			| TDATA TLBRACE decls TRBRACE
				{
					std::cout << "Data\n";
					$$ = $3;
				}

;

decls		: decl 
				{
					std::cout << "Single decl\n";
					$$ = new NData();
					$$->AddDecl($1);
				}
			| decls decl
				{
					std::cout << "Multiple decls\n";
					$$ = $1;
					$$->AddDecl($2);
				}
;

decl		: TVAR TIDENTIFIER TSEMI
				{
					std::cout << "Var declaration " << *($2) << '\n';
					$$ = new NVarDecl(*($2));

				}
			| TARRAY TIDENTIFIER TLBRACKET numeric TRBRACKET TSEMI
				{
					std::cout << "Array declaration " << *($2) << '\n';
					$$ = new NArrayDecl(*($2), $4);
				}
;
statements	: statement 
				{
					std::cout << "Single statement\n";
					$$ = new NBlock();
                			$$-> AddStatement($1);	
				}
			| statements statement
				{
					std::cout << "Multiple statements\n";
					$$ = $1;
					$$-> AddStatement($2);
				}

;

statement	: TIDENTIFIER TEQUALS expr TSEMI
				{
					std::cout << "Variable assignment "  << '\n';
					$$ = new NAssignVarStmt(*($1), $3);
				}
			| TIDENTIFIER TLBRACKET expr TRBRACKET TEQUALS expr TSEMI
				{
					std::cout << "assignment to a value at an array index " << '\n';
					$$ = new NAssignArrayStmt(*($1), $3, $6);
				}
			| TINC TIDENTIFIER TSEMI
				{
					std::cout << "increment " << '\n';
					$$ = new NIncStmt(*($2));
				}
			| TDEC TIDENTIFIER TSEMI
				{
					std::cout << "decrement " << '\n';
					$$ = new NDecStmt(*($2));
				}
			| TIF comparison block
				{
					std::cout << "if statement " << '\n';
					$$ = new NIfStmt($2, $3, nullptr);
				}
			| TIF comparison block TELSE block
				{
					std::cout << "if statement with else " << '\n';
					$$ = new NIfStmt($2, $3, $5);
				}
			| TWHILE comparison block
				{
					std::cout << "while loop " << '\n';
					$$ = new NWhileStmt($2, $3);

				}
			| TPENUP TLPAREN TRPAREN TSEMI
				{
					std::cout << "pen up " << '\n';
					$$ = new NPenUpStmt();
				}
			| TPENDOWN TLPAREN TRPAREN TSEMI
				{
					std::cout << "pen down " << '\n';
					$$ = new NPenDownStmt();
				}
			| TSETPOS TLPAREN expr TCOMMA expr TRPAREN TSEMI
				{
					std::cout << "set position " << '\n';
					$$ = new NSetPosStmt($3, $5);
				}
			| TSETCOLOR TLPAREN expr TRPAREN TSEMI
				{
					std::cout << "set color " << '\n';
					$$ = new NSetColorStmt($3);
				}
			| TFWD TLPAREN expr TRPAREN TSEMI
				{
					std::cout << "set forward " << '\n';
					$$ = new NFwdStmt($3);
				}
			| TBACK TLPAREN expr TRPAREN TSEMI
				{
					std::cout << "set back " << '\n';
					$$ = new NBackStmt($3);
				}
			| TROT TLPAREN expr TRPAREN TSEMI
				{
					std::cout << "rotate " << '\n';
					$$ = new NRotStmt($3);
				}
;

main		: TMAIN TLBRACE TRBRACE
				{
					std::cout << "Main (no stmts)\n";
					$$ = new NBlock();
				}
			| TMAIN TLBRACE block TRBRACE
				{
					std::cout << "Main containing a block\n";
					$$ = $3;

				}

;

expr		: expr TADD expr 
				{ 
					std::cout << "+" << std::endl;
					$$ = new NBinaryExpr($1, $2, $3);
				}
			| expr TSUB expr 
				{ 
					std::cout << "-" << std::endl; 
					$$ = new NBinaryExpr($1, $2, $3);
				}
			| expr TMUL expr 
				{ 
					std::cout << "*" << std::endl;
					$$ = new NBinaryExpr($1, $2, $3);
				}
			| expr TDIV expr 
				{ 
					std::cout << "/" << std::endl;
					$$ = new NBinaryExpr($1, $2, $3);
				}
			| TLPAREN expr TRPAREN 
				{ 
					std::cout << "()" << std::endl;
					$$ = $2;
				}
			| numeric 
				{
					std::cout << "Numeric expression\n";
					$$ = new NNumericExpr($1);

				}
			| TIDENTIFIER
				{
					std::cout << "variable expression\n";
					$$ = new NVarExpr(*($1));
				}
			| TIDENTIFIER TLBRACKET expr TRBRACKET
				{
					std::cout << "array expression\n";
					$$ = new NArrayExpr(*($1), $3);
				}

;

numeric		: TINTEGER
				{
					std::cout << "Numeric value of " << *($1) << '\n';
					$$ = new NNumeric(*($1));

				}
;


block		: statements
           			{
					std::cout << "block w/ statements no braces" << '\n';
           			}

			| TLBRACE statements TRBRACE
				{
					std::cout << "block" << '\n';
					$$ = $2;

				}


;

comparison	: expr TLESS expr
				{
					std::cout << "less than comparison " << '\n';
					$$ = new NComparison($1, $2, $3);
				}
		| expr TISEQUAL expr
				{
					std::cout << "equal to comparison " << '\n';
					$$ = new NComparison($1, $2, $3);
				}	
;
%%
