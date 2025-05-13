%{

#include "FlexActions.h"

static YY_BUFFER_STATE importedExpressionBufferState = NULL;

%}

/**
 * Enable start condition manipulation functions.
 */
%option stack

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */
%x IMPORT_EXPRESSION
%x MULTILINE_COMMENT

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

%%

"/*"								{ BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>"*/"				{ EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<MULTILINE_COMMENT>[[:space:]]+		{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>[^*]+			{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>.				{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }

"open"              				{ return ImageOperatorLexemeAction(createLexicalAnalyzerContext(), OPEN); }
"save"              				{ return ImageOperatorLexemeAction(createLexicalAnalyzerContext(), SAVE); }

"crop"              				{ return ImageOperatorLexemeAction(createLexicalAnalyzerContext(), CROP); }
"resize"            				{ return ImageOperatorLexemeAction(createLexicalAnalyzerContext(), RESIZE); }
"rotate"            				{ return ImageOperatorLexemeAction(createLexicalAnalyzerContext(), ROTATE); }
"flip"              				{ return ImageOperatorLexemeAction(createLexicalAnalyzerContext(), FLIP); }
"merge"             				{ return ImageOperatorLexemeAction(createLexicalAnalyzerContext(), MERGE); }

"brightness"        				{ return ImageOperatorLexemeAction(createLexicalAnalyzerContext(), BRIGHTNESS); }
"contrast"          				{ return ImageOperatorLexemeAction(createLexicalAnalyzerContext(), CONTRAST); }
"opacity"           				{ return ImageOperatorLexemeAction(createLexicalAnalyzerContext(), OPACITY); }
"invert"            				{ return ImageOperatorLexemeAction(createLexicalAnalyzerContext(), INVERT); }
"grayscale"         				{ return ImageOperatorLexemeAction(createLexicalAnalyzerContext(), GRAYSCALE); }
"recolor"           				{ return ImageOperatorLexemeAction(createLexicalAnalyzerContext(), RECOLOR); }

"blur"              				{ return ImageOperatorLexemeAction(createLexicalAnalyzerContext(), BLUR); }
"sharpen"           				{ return ImageOperatorLexemeAction(createLexicalAnalyzerContext(), SHARPEN); }
"pixelate"          				{ return ImageOperatorLexemeAction(createLexicalAnalyzerContext(), PIXELATE); }

"blend"             				{ return ImageOperatorLexemeAction(createLexicalAnalyzerContext(), BLEND); }

"horizontally"						{ return ImageOperatorLexemeAction(createLexicalAnalyzerContext(), HORIZONTALLY); }
"vertically"						{ return ImageOperatorLexemeAction(createLexicalAnalyzerContext(), VERTICALLY); }

"in"                				{ return ImageOperatorLexemeAction(createLexicalAnalyzerContext(), IN); }
"get"               				{ return ImageOperatorLexemeAction(createLexicalAnalyzerContext(), GET); }
"to"                				{ return ImageOperatorLexemeAction(createLexicalAnalyzerContext(), TO); }
"by"                				{ return ImageOperatorLexemeAction(createLexicalAnalyzerContext(), BY); }
"with"              				{ return ImageOperatorLexemeAction(createLexicalAnalyzerContext(), WITH); }
"using"             				{ return ImageOperatorLexemeAction(createLexicalAnalyzerContext(), USING); }
"on"                				{ return ImageOperatorLexemeAction(createLexicalAnalyzerContext(), ON); }

(100|[1-9]?[0-9])%					{ return PercentageLexemeAction(createLexicalAnalyzerContext()); }

[[:digit:]]+x[[:digit:]]+ 			{ return DimensionLexemeAction(createLexicalAnalyzerContext()); }

#[0-9A-Fa-f]{6}                     { return ColorLexemeAction(createLexicalAnalyzerContext()); }

"("									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }
")"									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS); }

\"[^"\n]*\"    						{ return StringLexemeAction(createLexicalAnalyzerContext()); }

";"									{ return SemicolonLexemeAction(createLexicalAnalyzerContext()); }

-?[[:digit:]]+						{ return IntegerLexemeAction(createLexicalAnalyzerContext()); }

[[:space:]]+						{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }

.									{ return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%

#include "FlexExport.h"
