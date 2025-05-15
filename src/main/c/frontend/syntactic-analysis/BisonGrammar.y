%{

#include "BisonActions.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

typedef char * String;

%union {
	/** Terminals. */

	int integer;
	Token token;
	String string;

	/** Non-terminals. */

	Constant * constant;
	Expression * expression;
	Factor * factor;
	Program * program;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach for the AST root node ("program" non-terminal, in this
 * grammar), or it will drop the entire tree even if the parse succeeds.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
%destructor { releaseConstant($$); } <constant>
%destructor { releaseExpression($$); } <expression>
%destructor { releaseFactor($$); } <factor>

/** Terminals. */
/** %token <token> ADD */
/** %token <token> DIV */
/** %token <token> MUL */
/** %token <token> SUB */

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
%token <token> HORIZONTALLY
%token <token> VERTICALLY
%token <token> IN
%token <token> GET
%token <token> TO
%token <token> BY
%token <token> WITH
%token <token> USING
%token <token> ON
%token <token> SEMICOLON
%token <token> UNKNOWN

/** Non-terminals. */
%type <constant> constant
%type <expression> expression
%type <factor> factor
%type <program> program

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 * %left ADD SUB
 * %left MUL DIV
*/

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program: expression													{ $$ = ExpressionProgramSemanticAction(currentCompilerState(), $1); }
	;

expression: OPEN string SEMICOLON									{ $$ = OpenImageExpressionSemanticAction($2); }
	| SAVE factor string SEMICOLON						{ $$ = SaveImageExpressionSemanticAction($2, $3); }
	| CROP factor IN integer GET integer SEMICOLON		{ $$ = CropImageExpressionSemanticAction($2, $4, $6); }
	| RESIZE factor TO dimension SEMICOLON				{ $$ = ResizeImageExpressionSemanticAction($2, $4); }
	| ROTATE factor BY integer SEMICOLON					{ $$ = RotateImageExpressionSemanticAction($2, $4); }
	| BRIGHTNESS factor BY integer SEMICOLON				{ $$ = BrightnessImageExpressionSemanticAction($2, $4); }
	| CONTRAST factor BY integer SEMICOLON				{ $$ = ContrastImageExpressionSemanticAction($2, $4); }
	| BLUR factor BY integer SEMICOLON					{ $$ = BlurImageExpressionSemanticAction($2, $4); }
	| PIXELATE factor BY integer SEMICOLON				{ $$ = PixelateImageExpressionSemanticAction($2, $4); }
	| OPACITY factor percentage SEMICOLON				{ $$ = OpacityImageExpressionSemanticAction($2, $3); }
	| FLIP factor orientation SEMICOLON					{ $$ = FlipImageExpressionSemanticAction($2, $3); }
	| GRAYSCALE factor SEMICOLON							{ $$ = GrayscaleImageExpressionSemanticAction($2); }
	| INVERT factor SEMICOLON							{ $$ = InvertImageExpressionSemanticAction($2); }
	| SHARPEN factor SEMICOLON							{ $$ = SharpenImageExpressionSemanticAction($2); }
	| BLEND factor WITH factor USING integer SEMICOLON	{ $$ = BlendImageExpressionSemanticAction($2, $4, $6); }
	| MERGE factor WITH factor orientation SEMICOLON		{ $$ = MergeImageExpressionSemanticAction($2, $4, $5); }
	| RECOLOR factor color color TO color SEMICOLON		{ $$ = RecolorImageExpressionSemanticAction($2, $3, $4, $6); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS	{ $$ = ExpressionSemanticAction($2); }
	;

integer: INTEGER													{ $$ = IntegerConstantSemanticAction($1); }
	;

string: STRING														{ $$ = StringConstantSemanticAction($1); }
	;

dimension: DIMENSION													{ $$ = DimensionConstantSemanticAction($1); }
	;

percentage: PERCENTAGE													{ $$ = PercentageConstantSemanticAction($1); }
	;

orientation: HORIZONTALLY												{ $$ = OrientationConstantSemanticAction($1); }
	| VERTICALLY														{ $$ = OrientationConstantSemanticAction($1); }
	;

color: COLOR															{ $$ = ColorConstantSemanticAction($1); }
	;
%%

// TODO: borrar esto
factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS				{ $$ = ExpressionFactorSemanticAction($2); }
	| constant														{ $$ = ConstantFactorSemanticAction($1); }
	;

expression: expression[left] ADD expression[right]					{ $$ = ArithmeticExpressionSemanticAction($left, $right, ADDITION); }
	| expression[left] DIV expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, DIVISION); }
	| expression[left] MUL expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, MULTIPLICATION); }
	| expression[left] SUB expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, SUBTRACTION); }
	| factor														{ $$ = FactorExpressionSemanticAction($1); }
	| FLIP expression HORIZONTALLY
	;