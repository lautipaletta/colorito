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
				if(expression->data.open.filename != NULL) free((void*) expression->data.open.filename);
				break;
	
			case SAVE_IMAGE:
				releaseFactor(expression->data.save.image);
				if(expression->data.save.filename != NULL) free((void*) expression->data.save.filename);
				break;
	
			case CROP_IMAGE:
				releaseFactor(expression->data.crop.image);
				break;
	
			case RESIZE_IMAGE:
				releaseFactor(expression->data.resize.image);
				if(expression->data.resize.dimension != NULL) free((void*) expression->data.resize.dimension);
				break;
	
			case ROTATE_IMAGE:
			case BRIGHTNESS_IMAGE:
			case CONTRAST_IMAGE:
			case BLUR_IMAGE:
			case PIXELATE_IMAGE:
			case OPACITY_IMAGE:
				releaseFactor(expression->data.numeric_op.image);
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
			case MERGE_IMAGES:
				releaseFactor(expression->data.dual_op.image1);
				releaseFactor(expression->data.dual_op.image2);
				break;
	
			case RECOLOR_IMAGE:
				releaseFactor(expression->data.recolor.image);
				if(expression->data.recolor.from_color1 != NULL) free((void*) expression->data.recolor.from_color1);
				if(expression->data.recolor.from_color2 != NULL) free((void*) expression->data.recolor.from_color2);
				if(expression->data.recolor.to_color != NULL) free((void*) expression->data.recolor.to_color);
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
		// if (line->expression != NULL) {
        //     releaseExpression(line->expression);
        //     line->expression = NULL; // Evitar doble liberación
        // }
        // Line *nextLine = line->next;
        // line->next = NULL; // Evitar que el puntero quede colgando
        // releaseLine(nextLine);
        // free(line);
		if(line->type == LINE_EXPRESSION) {
			releaseExpression(line->content.expression);
		} else {
			free((void*) line->content.variable_declaration.identifier);
			releaseExpression(line->content.variable_declaration.expression);
		}
		releaseLine(line->next);
		free(line);
	}
}
