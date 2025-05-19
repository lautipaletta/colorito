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
	Line * line
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
%type <orientation> orientation

/** Non-terminals. */
%type <expression> expression
%type <factor> factor
%type <program> program
%type <line> line

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
*/

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program: line											{ $$ = ExpressionProgramSemanticAction(currentCompilerState(), $1); }
	;

line: expression SEMICOLON 								{ $$ = ExpressionLineSemanticAction($1, NULL); }
	| expression SEMICOLON  line						{ $$ = ExpressionLineSemanticAction($1, $3); }
	;

expression: OPEN STRING									{ $$ = OpenImageExpressionSemanticAction($2); }
	| SAVE factor STRING								{ $$ = SaveImageExpressionSemanticAction($2, $3); }
	| CROP factor IN INTEGER GET INTEGER				{ $$ = CropImageExpressionSemanticAction($2, $4, $6); }
	| RESIZE factor TO DIMENSION						{ $$ = ResizeImageExpressionSemanticAction($2, $4); }
	| ROTATE factor BY INTEGER 							{ $$ = RotateImageExpressionSemanticAction($2, $4); }
	| BRIGHTNESS factor BY INTEGER						{ $$ = BrightnessImageExpressionSemanticAction($2, $4); }
	| CONTRAST factor BY INTEGER						{ $$ = ContrastImageExpressionSemanticAction($2, $4); }
	| BLUR factor BY INTEGER							{ $$ = BlurImageExpressionSemanticAction($2, $4); }
	| PIXELATE factor BY INTEGER						{ $$ = PixelateImageExpressionSemanticAction($2, $4); }
	| OPACITY factor PERCENTAGE							{ $$ = OpacityImageExpressionSemanticAction($2, $3); }
	| FLIP factor orientation							{ $$ = FlipImageExpressionSemanticAction($2, $3); }
	| GRAYSCALE factor									{ $$ = GrayscaleImageExpressionSemanticAction($2); }
	| INVERT factor										{ $$ = InvertImageExpressionSemanticAction($2); }
	| SHARPEN factor									{ $$ = SharpenImageExpressionSemanticAction($2); }
	| BLEND factor WITH factor USING INTEGER			{ $$ = BlendImageExpressionSemanticAction($2, $4, $6); }
	| MERGE factor WITH factor orientation				{ $$ = MergeImageExpressionSemanticAction($2, $4, $5); }
	| RECOLOR factor COLOR COLOR TO COLOR				{ $$ = RecolorImageExpressionSemanticAction($2, $3, $4, $6); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS	{ $$ = ExpressionSemanticAction($2); }
	;

orientation: HORIZONTALLY								{ $$ = HORIZONTAL; }
	| VERTICALLY										{ $$ = VERTICAL; }
	;
%%