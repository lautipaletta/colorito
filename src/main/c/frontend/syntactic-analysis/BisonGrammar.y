%{

#include "BisonActions.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
	/** Terminals. */

	int integer;
	Token token;
	char * string;
	Orientation orientation;

	/** Non-terminals. */

	Expression * expression;
	Factor * factor;
	Program * program;
	Line * line;
	Variable * variable;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach for the AST root node ("program" non-terminal, in this
 * grammar), or it will drop the entire tree even if the parse succeeds.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */

%destructor { releaseProgram($$); } <program>
%destructor { releaseExpression($$); } <expression>
%destructor { releaseFactor($$); } <factor>

/** Terminals. */
%token <string> STRING
%token <string> DIMENSION
%token <string> COLOR
%token <integer> INTEGER
%token <integer> PERCENTAGE
%token <token> CLOSE_PARENTHESIS
%token <token> OPEN_PARENTHESIS
%token <token> OPEN
%token <token> SAVE
%token <token> CROP
%token <token> RESIZE
%token <token> ROTATE
%token <token> FLIP
%token <token> MERGE
%token <token> BRIGHTNESS
%token <token> CONTRAST
%token <token> OPACITY
%token <token> INVERT
%token <token> GRAYSCALE
%token <token> RECOLOR
%token <token> BLUR
%token <token> SHARPEN
%token <token> PIXELATE
%token <token> BLEND
%token <token> EQUALS
%token <string> IDENTIFIER
%token <token> HORIZONTALLY
%token <token> VERTICALLY
%token <token> IN
%token <token> GET
%token <token> TO
%token <token> BY
%token <token> WITH
%token <token> USING
%token <token> SEMICOLON
%token <token> UNKNOWN
%type <orientation> orientation

/** Non-terminals. */
%type <expression> expression
%type <factor> factor
%type <program> program
%type <line> line
%type <variable> variable_string
%type <variable> variable_integer
%type <variable> variable_expression
%type <variable> variable_color
%type <variable> variable_percentage
%type <variable> variable_dimension
%type <variable> variable

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
*/

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program: line													{ $$ = ExpressionProgramSemanticAction(currentCompilerState(), $1); }
	;

line: expression SEMICOLON 										{ $$ = ExpressionLineSemanticAction($1, NULL); }
	| expression SEMICOLON  line								{ $$ = ExpressionLineSemanticAction($1, $3); }
	| IDENTIFIER EQUALS variable SEMICOLON						{ $$ = VariableDeclarationLineSemanticAction($1, $3, NULL); }
	| IDENTIFIER EQUALS variable SEMICOLON  line				{ $$ = VariableDeclarationLineSemanticAction($1, $3, $5); }
	;

variable: variable_string										{ $$ = $1; }
	| variable_integer											{ $$ = $1; }
	| variable_expression										{ $$ = $1; }
	| variable_color											{ $$ = $1; }
	| variable_percentage										{ $$ = $1; }
	| variable_dimension										{ $$ = $1; }
	;

expression: OPEN variable_string								{ $$ = OpenImageExpressionSemanticAction($2); }
	| SAVE factor variable_string								{ $$ = SaveImageExpressionSemanticAction($2, $3); }
	| CROP factor IN variable_integer GET variable_integer		{ $$ = CropImageExpressionSemanticAction($2, $4, $6); }
	| RESIZE factor TO variable_dimension						{ $$ = ResizeImageExpressionSemanticAction($2, $4); }
	| ROTATE factor BY variable_integer 						{ $$ = ExpressionWithIntegerSemanticAction($2, $4, ROTATE_IMAGE); }
	| BRIGHTNESS factor BY variable_integer						{ $$ = ExpressionWithIntegerSemanticAction($2, $4, BRIGHTNESS_IMAGE); }
	| CONTRAST factor BY variable_integer						{ $$ = ExpressionWithIntegerSemanticAction($2, $4, CONTRAST_IMAGE); }
	| BLUR factor BY variable_integer							{ $$ = ExpressionWithIntegerSemanticAction($2, $4, BLUR_IMAGE); }
	| PIXELATE factor BY variable_integer						{ $$ = ExpressionWithIntegerSemanticAction($2, $4, PIXELATE_IMAGE); }
	| OPACITY factor variable_percentage						{ $$ = ExpressionWithIntegerSemanticAction($2, $3, OPACITY_IMAGE); }
	| FLIP factor orientation									{ $$ = FlipImageExpressionSemanticAction($2, $3); }
	| GRAYSCALE factor											{ $$ = ExpressionWithOnlyFactorSemanticAction($2, GRAYSCALE_IMAGE); }
	| INVERT factor												{ $$ = ExpressionWithOnlyFactorSemanticAction($2, INVERT_IMAGE); }
	| SHARPEN factor											{ $$ = ExpressionWithOnlyFactorSemanticAction($2, SHARPEN_IMAGE); }
	| BLEND factor WITH factor USING variable_percentage		{ $$ = BlendImageExpressionSemanticAction($2, $4, $6); }
	| MERGE factor WITH factor orientation						{ $$ = MergeImageExpressionSemanticAction($2, $4, $5); }
	| RECOLOR factor variable_color variable_color TO variable_color	{ $$ = RecolorImageExpressionSemanticAction($2, $3, $4, $6); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS	{ $$ = ExpressionSemanticAction($2); }
	| IDENTIFIER										{ $$ = VariableFactorSemanticAction($1); }
	;

variable_string: STRING									{ $$ = StringVariableSemanticAction($1); }
	| IDENTIFIER                                           		{ $$ = IdentifierVariableSemanticAction($1); }
	;

variable_integer: INTEGER								{ $$ = IntegerVariableSemanticAction($1); }
	| IDENTIFIER                                           		{ $$ = IdentifierVariableSemanticAction($1); }
	;

variable_expression: expression							{ $$ = ExpressionVariableSemanticAction($1); }
	| IDENTIFIER                                           		{ $$ = IdentifierVariableSemanticAction($1); }
	;

variable_color: COLOR									{ $$ = ColorVariableSemanticAction($1); }
	| IDENTIFIER                                           		{ $$ = IdentifierVariableSemanticAction($1); }
	;

variable_percentage: PERCENTAGE							{ $$ = PercentageVariableSemanticAction($1); }
	| IDENTIFIER                                           		{ $$ = IdentifierVariableSemanticAction($1); }
	;

variable_dimension: DIMENSION							{ $$ = DimensionVariableSemanticAction($1); }
	| IDENTIFIER                                           		{ $$ = IdentifierVariableSemanticAction($1); }
	;

orientation: HORIZONTALLY								{ $$ = HORIZONTAL; }
	| VERTICALLY										{ $$ = VERTICAL; }
	;
%%