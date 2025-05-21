#include "BisonActions.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeBisonActionsModule() {
	_logger = createLogger("BisonActions");
}

void shutdownBisonActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** IMPORTED FUNCTIONS */

extern unsigned int flexCurrentContext(void);

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char * functionName);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char * functionName) {
	logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */

Program * ExpressionProgramSemanticAction(CompilerState * compilerState, Line * line) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->line = line;
	compilerState->abstractSyntaxtTree = program;
	if (0 < flexCurrentContext()) {
		logError(_logger, "The final context is not the default (0): %d", flexCurrentContext());
		compilerState->succeed = false;
	}
	else {
		compilerState->succeed = true;
	}
	return program;
}

Line * ExpressionLineSemanticAction(Expression * expression, Line * next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Line * line = calloc(1, sizeof(Line));
	line->type = LINE_EXPRESSION;
	line->content.expression = expression;
	line->next = next;
	return line;
}

Line * VariableDeclarationLineSemanticAction(const char * identifier, Variable * variable, Line * next) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Line * line = calloc(1, sizeof(Line));
	line->type = LINE_VARIABLE_DECLARATION;
    line->content.variable_declaration.identifier = identifier;
    line->content.variable_declaration.variable = variable;
    line->next = next;
    return line;
}

Factor * ExpressionSemanticAction(Expression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->type = FACTOR_EXPRESSION;
	factor->data.expression = expression;
	return factor;
}

Factor * VariableFactorSemanticAction(const char * identifier) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->type = FACTOR_IDENTIFIER;
	factor->data.identifier = identifier;
	return factor;
}

Expression * OpenImageExpressionSemanticAction(Variable * filename){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * result = calloc(1, sizeof(Expression));
	result->data.open.filename = filename;
	result->type = OPEN_IMAGE;
	return result;
}

Expression * SaveImageExpressionSemanticAction(Factor * image, Variable * filename){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * result = calloc(1, sizeof(Expression));
	result->data.save.image = image;
	result->data.save.filename = filename;
	result->type = SAVE_IMAGE;
	return result;
}

Expression * CropImageExpressionSemanticAction(Factor * image, Variable * divisions_qty, Variable * output_division) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * result = calloc(1, sizeof(Expression));
	result->data.crop.image = image;
	result->data.crop.divisions_qty = divisions_qty;
	result->data.crop.output_division = output_division;
	result->type = CROP_IMAGE;
	return result;
}

Expression * ResizeImageExpressionSemanticAction(Factor * image, Variable * dimension) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * result = calloc(1, sizeof(Expression));
	result->data.resize.image = image;
	result->data.resize.dimension = dimension;
	result->type = RESIZE_IMAGE;
	return result;
}

Expression * ExpressionWithIntegerSemanticAction(Factor * image, Variable * integer, ExpressionType expressionType) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * result = calloc(1, sizeof(Expression));
	result->data.numeric_op.image = image;
	result->data.numeric_op.value = integer;
	result->type = expressionType;
	return result;
}

Expression * FlipImageExpressionSemanticAction(Factor * image, Orientation orientation) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * result = calloc(1, sizeof(Expression));
	result->data.directional_op.image = image;
	result->data.directional_op.direction = orientation;
	result->type = FLIP_IMAGE;
	return result;
}

Expression * ExpressionWithOnlyFactorSemanticAction(Factor * image, ExpressionType expressionType) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
 	Expression * result = calloc(1, sizeof(Expression));
 	result->data.simple_op.image = image;
 	result->type = expressionType;
 	return result;
}

Expression * BlendImageExpressionSemanticAction(Factor * image1, Factor * image2, Variable * amount) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * result = calloc(1, sizeof(Expression));
	result->data.dual_op.image1 = image1;
	result->data.dual_op.image2 = image2;
	result->data.dual_op.param.blend_factor = amount;
	result->type = BLEND_IMAGES;
	return result;
}

Expression * MergeImageExpressionSemanticAction(Factor * image1, Factor * image2, Orientation orientation) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * result = calloc(1, sizeof(Expression));
	result->data.dual_op.image1 = image1;
	result->data.dual_op.image2 = image2;
	result->data.dual_op.param.direction = orientation;
	result->type = MERGE_IMAGES;
	return result;
}

Expression * RecolorImageExpressionSemanticAction(Factor * image, Variable * sourceColor1, Variable * sourceColor2, Variable * targetColor) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * result = calloc(1, sizeof(Expression));
	result->data.recolor.image = image;
	result->data.recolor.from_color1 = sourceColor1;
	result->data.recolor.from_color2 = sourceColor2;
	result->data.recolor.to_color = targetColor;
	result->type = RECOLOR_IMAGE;
	return result;
}

Variable * StringVariableSemanticAction(const char * value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Variable * result = calloc(1, sizeof(Variable));
	result->type = STRING_TYPE;
	result->data.string = value;
	return result;
}

Variable * IntegerVariableSemanticAction(int value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Variable * result = calloc(1, sizeof(Variable));
	result->type = INTEGER_TYPE;
	result->data.integer = value;
	return result;
}

Variable * ExpressionVariableSemanticAction(Expression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Variable * result = calloc(1, sizeof(Variable));
	result->type = EXPRESSION_TYPE;
	result->data.expression = expression;
	return result;
}

Variable * ColorVariableSemanticAction(const char * value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Variable * result = calloc(1, sizeof(Variable));
	result->type = COLOR_TYPE;
	result->data.color = value;
	return result;
}

Variable * PercentageVariableSemanticAction(int value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Variable * result = calloc(1, sizeof(Variable));
	result->type = PERCENTAGE_TYPE;
	result->data.percentage = value;
	return result;
}

Variable * DimensionVariableSemanticAction(const char * value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Variable * result = calloc(1, sizeof(Variable));
	result->type = DIMENSION_TYPE;
	result->data.dimension = value;
	return result;
}

Variable * IdentifierVariableSemanticAction(const char * identifier) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Variable * result = calloc(1, sizeof(Variable));
	result->type = IDENTIFIER_TYPE;
	result->data.identifier = identifier;
	return result;
}
	
	