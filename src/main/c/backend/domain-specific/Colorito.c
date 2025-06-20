#include "Colorito.h"
static Logger * _logger = NULL;

void initializeColoritoModule() {
	_logger = createLogger("Colorito");
}

void shutdownColoritoModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/* === SYMBOL TABLE === */

typedef struct VariableEntry {
	const char * name;
	VariableType type;
	struct VariableEntry * next;
} VariableEntry;

// TODO: almacenar variables en el semantic value.
static VariableEntry * _symbolTable = NULL;

static void _clearSymbolTable() {
	VariableEntry * current = _symbolTable;
	while (current != NULL) {
		VariableEntry * next = current->next;
		free(current);
		current = next;
	}
	_symbolTable = NULL;
}

static void _declareVariable(const char * name, VariableType type) {
	VariableEntry * entry = malloc(sizeof(VariableEntry));
	entry->name = name;
	entry->type = type;
	entry->next = _symbolTable;
	_symbolTable = entry;
}

static VariableEntry * _findVariable(const char * name) {
	for (VariableEntry * current = _symbolTable; current != NULL; current = current->next) {
		if (strcmp(current->name, name) == 0) return current;
	}
	return NULL;
}

/* === TYPE CHECKING === */

static boolean _checkVariableWithType(Variable * var, VariableType expectedType) {
	if (!var) return false;
	if (var->type == IDENTIFIER_TYPE) {
		VariableEntry * entry = _findVariable(var->data.identifier);
		if (!entry) {
			logError(_logger, "Variable not declared: %s", var->data.identifier);
			return false;
		}
		if (entry->type != expectedType) {
			logError(_logger, "Incorrect type: expected %d but found %d (identifier %s)",
				expectedType, entry->type, var->data.identifier);
			return false;
		}
	} else if (var->type != expectedType) {
		logError(_logger, "Incorrect type in literal: expected %d but found %d", expectedType, var->type);
		return false;
	}
	return true;
}

static boolean _checkFactor(Factor * factor);
static boolean _checkExpression(Expression * expression);

static boolean _checkFactor(Factor * factor) {
	if (factor == NULL) return false;
	if (factor->type == FACTOR_EXPRESSION) {
		return _checkExpression(factor->data.expression);
	} else if (factor->type == FACTOR_IDENTIFIER) {
		Variable * var = calloc(1, sizeof(Variable));
		var->type = IDENTIFIER_TYPE;
		var->data.identifier = strdup(factor->data.identifier);
		boolean validFactor = _checkVariableWithType(var, EXPRESSION_TYPE);
		free((void *)var->data.identifier);
		free(var);
		if (!validFactor) {
			logError(_logger, "Identifier not declared or of invalid type: %s", factor->data.identifier);
			return false;
		}
	}
	return true;
}

static boolean _checkExpression(Expression * expr) {
	if (expr == NULL) return false;

	switch (expr->type) {
		case OPEN_IMAGE:
			return _checkVariableWithType(expr->data.open.filename, STRING_TYPE);

		case SAVE_IMAGE:
			return _checkFactor(expr->data.save.image) &&
			       _checkVariableWithType(expr->data.save.filename, STRING_TYPE);

		case CROP_IMAGE:
			return _checkFactor(expr->data.crop.image) &&
			       _checkVariableWithType(expr->data.crop.divisions_qty, INTEGER_TYPE) &&
			       _checkVariableWithType(expr->data.crop.output_division, INTEGER_TYPE);

		case RESIZE_IMAGE:
			return _checkFactor(expr->data.resize.image) &&
			       _checkVariableWithType(expr->data.resize.dimension, DIMENSION_TYPE);

		case ROTATE_IMAGE: case BRIGHTNESS_IMAGE: case CONTRAST_IMAGE:
		case BLUR_IMAGE: case PIXELATE_IMAGE:
			return _checkFactor(expr->data.numeric_op.image) &&
			       _checkVariableWithType(expr->data.numeric_op.value, INTEGER_TYPE);

		case OPACITY_IMAGE:
			return _checkFactor(expr->data.numeric_op.image) &&
			       _checkVariableWithType(expr->data.numeric_op.value, PERCENTAGE_TYPE);

		case FLIP_IMAGE:
			return _checkFactor(expr->data.directional_op.image);

		case GRAYSCALE_IMAGE: case INVERT_IMAGE: case SHARPEN_IMAGE:
			return _checkFactor(expr->data.simple_op.image);

		case BLEND_IMAGES:
			return _checkFactor(expr->data.dual_op.image1) &&
			       _checkFactor(expr->data.dual_op.image2) &&
			       _checkVariableWithType(expr->data.dual_op.param.blend_factor, PERCENTAGE_TYPE);

		case MERGE_IMAGES:
			return _checkFactor(expr->data.dual_op.image1) &&
			       _checkFactor(expr->data.dual_op.image2);

		case RECOLOR_IMAGE:
			return _checkFactor(expr->data.recolor.image) &&
			       _checkVariableWithType(expr->data.recolor.from_color1, COLOR_TYPE) &&
			       _checkVariableWithType(expr->data.recolor.from_color2, COLOR_TYPE) &&
			       _checkVariableWithType(expr->data.recolor.to_color, COLOR_TYPE);
	}
	return true;
}

static boolean _checkLine(Line * line) {
	if (line == NULL) return true;
	boolean result = true;

	switch (line->type) {
		case LINE_VARIABLE_DECLARATION: {
			const char * id = line->content.variable_declaration.identifier;
			Variable * var = line->content.variable_declaration.variable;

			if (_findVariable(id)) {
				logError(_logger, "Redeclared variable: %s", id);
				result = false;
			} else {
				_declareVariable(id, var->type);
				result = _checkVariableWithType(var, var->type);
			}
			break;
		}
		case LINE_EXPRESSION:
			result = _checkExpression(line->content.expression);
			break;
	}

	return result && _checkLine(line->next);
}

/* === PUBLIC FUNCTION === */

boolean computeProgram(Program * program) {
	logDebugging(_logger, "Starting semantic analysis...");
	_clearSymbolTable();
	boolean success = program != NULL &&
	               _checkLine(program->line);
	if (!success) {
		logError(_logger, "Semantic analysis failed.");
	} else {
		logDebugging(_logger, "Semantic analysis completed successfully.");
	}
	_clearSymbolTable();
	return success;
}
