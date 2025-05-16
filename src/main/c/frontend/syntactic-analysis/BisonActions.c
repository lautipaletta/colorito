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

Program * ExpressionProgramSemanticAction(CompilerState * compilerState, Expression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->expression = expression;
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

Expression * OpenImageExpressionSemanticAction (const char * filename){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * result = calloc(1, sizeof(Expression));
	result->data.open.filename = strdup(filename);
	result->type = OPEN_IMAGE;
	return result;
}

Expression * SaveImageExpressionSemanticAction (Expression * image, const char * filename){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * result = calloc(1, sizeof(Expression));
	result->data.save.image = image;
	result->data.save.filename = strdup(filename);
	result->type = SAVE_IMAGE;
	return result;
}

Expression * CropImageExpressionSemanticAction(Expression * image, int divisions_qty, int output_division) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * result = calloc(1, sizeof(Expression));
	result->data.crop.image = image;
	result->data.crop.divisions_qty = divisions_qty;
	result->data.crop.output_division = output_division;
	result->type = CROP_IMAGE;
	return result;
}

Expression * ResizeImageExpressionSemanticAction(Expression * image, const char * dimension) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * result = calloc(1, sizeof(Expression));
	result->data.resize.image = image;
	result->data.resize.dimension = strdup(dimension);
	result->type = RESIZE_IMAGE;
	return result;
}
Expression * RotateImageExpressionSemanticAction(Expression * image, int degrees) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * result = calloc(1, sizeof(Expression));
	result->data.numeric_op.image = image;
	result->data.numeric_op.value = degrees;
	result->type = ROTATE_IMAGE;
	return result;
}

Expression * BrightnessImageExpressionSemanticAction(Expression * image, int amount) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * result = calloc(1, sizeof(Expression));
	result->data.numeric_op.image = image;
	result->data.numeric_op.value = degrees;
	result->type = BRIGHTNESS_ADJ;
	return result;
}

Expression * ContrastImageExpressionSemanticAction(Expression * image, int amount) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * result = calloc(1, sizeof(Expression));
	result->data.numeric_op.image = image;
	result->data.numeric_op.value = degrees;
	result->type = CONTRAST_ADJ;
	return result;
}

Expression * BlurImageExpressionSemanticAction(Expression * image, int amount) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * result = calloc(1, sizeof(Expression));
	result->data.numeric_op.image = image;
	result->data.numeric_op.value = degrees;
	result->type = BLUR_IMAGE;
	return result;
}

Expression * PixelateImageExpressionSemanticAction(Expression * image, int amount) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * result = calloc(1, sizeof(Expression));
	result->data.numeric_op.image = image;
	result->data.numeric_op.value = degrees;
	result->type = PIXELATE_IMAGE;
	return result;
}

Expression * OpacityImageExpressionSemanticAction(Expression * image, int percentage) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * result = calloc(1, sizeof(Expression));
	result->data.numeric_op.image = image;
	result->data.numeric_op.value = degrees;
	result->type = OPACITY_ADJ;
	return result;
}

Expression * FlipImageExpressionSemanticAction(Expression * image, DirectionType orientation) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * result = calloc(1, sizeof(Expression));
	result->data.directional_op.image = image;
	result->data.directional_op.direction = orientation;
	result->type = FLIP_IMAGE;
	return result;
}

Expression * GrayscaleImageExpressionSemanticAction(Expression * image) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * result = calloc(1, sizeof(Expression));
	result->data.simple_op.image = image;
	result->type = GRAYSCALE_IMAGE;
	return result;
}

Expression * InvertImageExpressionSemanticAction(Expression * image) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * result = calloc(1, sizeof(Expression));
	result->data.simple_op.image = image;
	result->type = INVERT_IMAGE;
	return result;
}

Expression * SharpenImageExpressionSemanticAction(Expression * image) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * result = calloc(1, sizeof(Expression));
	result->data.simple_op.image = image;
	result->type = SHARPEN_IMAGE;
	return result;
}

Expression * BlendImageExpressionSemanticAction(Expression * image1, Expression * image2, int amount) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * result = calloc(1, sizeof(Expression));
	result->data.dual_op.image1 = image1;
	result->data.dual_op.image2 = image2;
	result->data.dual_op.param.blend_factor = amount;
	result->type = BLEND_IMAGES;
	return result;
}
Expression * MergeImageExpressionSemanticAction(Expression * image1, Expression * image2, DirectionType orientation) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * result = calloc(1, sizeof(Expression));
	result->data.dual_op.image1 = image1;
	result->data.dual_op.image2 = image2;
	result->data.dual_op.param.direction = orientation;
	result->type = MERGE_IMAGES;
	return result;
}
Expression * RecolorImageExpressionSemanticAction(Expression * image, const char * sourceColor1, const char * sourceColor2, const char * targetColor) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * result = calloc(1, sizeof(Expression));
	result->data.recolor.image = image;
	result->data.recolor.from_color1 = strdup(sourceColor1);
	result->data.recolor.from_color2 = strdup(sourceColor2);
	result->data.recolor.to_color = strdup(targetColor);
	result->type = RECOLOR_IMAGE;
	return result;
}
 

// TODO: liberar memoria que se reserva en strings

// void destroySemanticValue(union SemanticValue* value, Token token) {
//     if (value != NULL) {
//         if (token == STRING && value->string != NULL) {
//             free(value->string);
//             value->string = NULL;
//         }
//         // Limpiar otros tipos si es necesario
//     }
// }

/*

Factor * ConstantFactorSemanticAction(Constant * constant) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->constant = constant;
	factor->type = CONSTANT;
	return factor;
}

Factor * ExpressionFactorSemanticAction(Expression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->expression = expression;
	factor->type = EXPRESSION;
	return factor;
}

Expression * ArithmeticExpressionSemanticAction(Expression * leftExpression, Expression * rightExpression, ExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->leftExpression = leftExpression;
	expression->rightExpression = rightExpression;
	expression->type = type;
	return expression;
}

Expression * FactorExpressionSemanticAction(Factor * factor) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->factor = factor;
	expression->type = FACTOR;
	return expression;
}

Constant * IntegerConstantSemanticAction(const int value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant * constant = calloc(1, sizeof(Constant));
	constant->value = value;
	return constant;
}
*/