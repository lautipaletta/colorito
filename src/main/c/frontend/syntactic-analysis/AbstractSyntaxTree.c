#include "AbstractSyntaxTree.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeAbstractSyntaxTreeModule() {
	_logger = createLogger("AbstractSyntxTree");
}

void shutdownAbstractSyntaxTreeModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PUBLIC FUNCTIONS */

void releaseExpression(Expression * expression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	logDebugging(_logger, "Liberando expresion en %p", expression);
	if (expression != NULL) {
		switch (expression->type) {
			case OPEN_IMAGE:
				releaseVariable(expression->data.open.filename);
				break;
	
			case SAVE_IMAGE:
				releaseFactor(expression->data.save.image);
				releaseVariable(expression->data.save.filename);
				break;
	
			case CROP_IMAGE:
				releaseFactor(expression->data.crop.image);
				releaseVariable(expression->data.crop.divisions_qty);
				releaseVariable(expression->data.crop.output_division);
				break;
	
			case RESIZE_IMAGE:
				releaseFactor(expression->data.resize.image);
				releaseVariable(expression->data.resize.dimension);
				break;
	
			case ROTATE_IMAGE:
			case BRIGHTNESS_IMAGE:
			case CONTRAST_IMAGE:
			case BLUR_IMAGE:
			case PIXELATE_IMAGE:
			case OPACITY_IMAGE:
				releaseFactor(expression->data.numeric_op.image);
				releaseVariable(expression->data.numeric_op.value);
				break;
	
			case FLIP_IMAGE:
				releaseFactor(expression->data.directional_op.image);
				break;
	
			case GRAYSCALE_IMAGE:
			case INVERT_IMAGE:
			case SHARPEN_IMAGE:
				releaseFactor(expression->data.simple_op.image);
				break;
	
			case BLEND_IMAGES:
				releaseFactor(expression->data.dual_op.image1);
				releaseFactor(expression->data.dual_op.image2);
				releaseVariable(expression->data.dual_op.param.blend_factor);
				break;
			case MERGE_IMAGES:
				releaseFactor(expression->data.dual_op.image1);
				releaseFactor(expression->data.dual_op.image2);
				break;
			case RECOLOR_IMAGE:
				releaseFactor(expression->data.recolor.image);
				releaseVariable(expression->data.recolor.from_color1);
				releaseVariable(expression->data.recolor.from_color2);
				releaseVariable(expression->data.recolor.to_color);
				break;
		}
		free(expression);
	}
}

void releaseProgram(Program * program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	logDebugging(_logger, "Liberando programa en %p", program);
	if (program != NULL) {
		releaseLine(program->line);
		free(program);
	}
}

void releaseFactor(Factor * factor) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	logDebugging(_logger, "Liberando factor en %p", factor);
	if (factor != NULL) {
		if(factor->type == FACTOR_EXPRESSION) {
			releaseExpression(factor->data.expression);
		} else {
			free((void*) factor->data.identifier);
		}
		free(factor);
	}
}

void releaseLine(Line * line) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	logDebugging(_logger, "Liberando line en %p", line);
	if (line != NULL) {
		switch(line->type) {
			case LINE_EXPRESSION:
				releaseExpression(line->content.expression);
				break;
			case LINE_VARIABLE_DECLARATION:
				free((void*) line->content.variable_declaration.identifier);
				releaseVariable(line->content.variable_declaration.variable);
				break;
		}
		releaseLine(line->next);
		free(line);
	}
}

void releaseVariable(Variable * variable) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	logDebugging(_logger, "Liberando variable en %p", variable);
	if (variable != NULL) {
		switch (variable->type) {
			case STRING_TYPE:
				free((void*) variable->data.string);
				break;
			case INTEGER_TYPE:
				break;
			case EXPRESSION_TYPE:
				releaseExpression(variable->data.expression);
				break;
			case COLOR_TYPE:
				free((void*) variable->data.color);
				break;
			case PERCENTAGE_TYPE:
				break;
			case DIMENSION_TYPE:
				free((void*) variable->data.dimension);
				break;
			case IDENTIFIER_TYPE:
				free((void*) variable->data.identifier);
				break;
		}
		free(variable);
	}
}

