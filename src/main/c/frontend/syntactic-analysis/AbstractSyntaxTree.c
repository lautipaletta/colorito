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
	if (expression != NULL) {
		switch (expression->type) {
			case OPEN_IMAGE:
				free(expression->data.open.filename);
				break;
	
			case SAVE_IMAGE:
				releaseExpression(expression->data.save.image);
				free(expression->data.save.filename);
				break;
	
			case CROP_IMAGE:
				releaseExpression(expression->data.crop.image);
				break;
	
			case RESIZE_IMAGE:
				releaseExpression(expression->data.resize.image);
				free(expression->data.resize.dimension);
				break;
	
			case ROTATE_IMAGE:
			case BRIGHTNESS_ADJ:
			case CONTRAST_ADJ:
			case BLUR_IMAGE:
			case PIXELATE_IMAGE:
			case OPACITY_ADJ:
				releaseExpression(expression->data.numeric_op.image);
				break;
	
			case FLIP_IMAGE:
				releaseExpression(expression->data.directional_op.image);
				break;
	
			case GRAYSCALE_IMAGE:
			case INVERT_IMAGE:
			case SHARPEN_IMAGE:
				releaseExpression(expression->data.simple_op.image);
				break;
	
			case BLEND_IMAGES:
			case MERGE_IMAGES:
				releaseExpression(expression->data.dual_op.image1);
				releaseExpression(expression->data.dual_op.image2);
				break;
	
			case RECOLOR_IMAGE:
				releaseExpression(expression->data.recolor.image);
				free(expression->data.recolor.from_color1);
				free(expression->data.recolor.from_color2);
				free(expression->data.recolor.to_color);
				break;
		}
		free(expression);
	}
}

void releaseProgram(Program * program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (program != NULL) {
		releaseExpression(program->expression);
		free(program);
	}
}

void releaseFactor(Factor * factor) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (factor != NULL) {
		releaseExpression(factor->expression);
		free(factor);
	}
}