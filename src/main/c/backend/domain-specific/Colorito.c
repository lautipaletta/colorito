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
			logError(_logger, "Variable no declarada: %s", var->data.identifier);
			return false;
		}
		if (entry->type != expectedType) {
			logError(_logger, "Tipo incorrecto: se esperaba %d pero se encontró %d (identificador %s)",
				expectedType, entry->type, var->data.identifier);
			return false;
		}
	} else if (var->type != expectedType) {
		logError(_logger, "Tipo incorrecto en literal: se esperaba %d pero se encontró %d", expectedType, var->type);
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
			logError(_logger, "Identificador no declarado o de tipo inválido: %s", factor->data.identifier);
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
				logError(_logger, "Redeclaración de variable: %s", id);
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
	logDebugging(_logger, "Iniciando análisis semántico...");
	_clearSymbolTable();
	boolean success = program != NULL &&
	               _checkLine(program->line);
	if (!success) {
		logError(_logger, "Análisis semántico fallido.");
	} else {
		logDebugging(_logger, "Análisis semántico finalizado exitosamente.");
	}
	_clearSymbolTable();
	return success;
}

// /** PRIVATE FUNCTIONS */

// static BinaryOperator _expressionTypeToBinaryOperator(const ExpressionType type);
// static ComputationResult _invalidBinaryOperator(const int x, const int y);
// static ComputationResult _invalidComputation();

// /**
//  * Converts and expression type to the proper binary operator. If that's not
//  * possible, returns a binary operator that always returns an invalid
//  * computation result.
//  */
// static BinaryOperator _expressionTypeToBinaryOperator(const ExpressionType type) {
// 	switch (type) {
// 		case ADDITION: return add;
// 		case DIVISION: return divide;
// 		case MULTIPLICATION: return multiply;
// 		case SUBTRACTION: return subtract;
// 		default:
// 			logError(_logger, "The specified expression type cannot be converted into character: %d", type);
// 			return _invalidBinaryOperator;
// 	}
// }

// /**
//  * A binary operator that always returns an invalid computation result.
//  */
// static ComputationResult _invalidBinaryOperator(const int x, const int y) {
// 	return _invalidComputation();
// }

// /**
//  * A computation that always returns an invalid result.
//  */
// static ComputationResult _invalidComputation() {
// 	ComputationResult computationResult = {
// 		.succeed = false,
// 		.value = 0
// 	};
// 	return computationResult;
// }

// /** PUBLIC FUNCTIONS */

// ComputationResult add(const int leftAddend, const int rightAddend) {
// 	ComputationResult computationResult = {
// 		.succeed = true,
// 		.value = leftAddend + rightAddend
// 	};
// 	return computationResult;
// }

// ComputationResult divide(const int dividend, const int divisor) {
// 	const int sign = dividend < 0 ? -1 : +1;
// 	const boolean divisionByZero = divisor == 0 ? true : false;
// 	if (divisionByZero) {
// 		logError(_logger, "The divisor cannot be zero (the computation was %d/%d).", dividend, divisor);
// 	}
// 	ComputationResult computationResult = {
// 		.succeed = divisionByZero ? false : true,
// 		.value = divisionByZero ? (sign * INT_MAX) : (dividend / divisor)
// 	};
// 	return computationResult;
// }

// ComputationResult multiply(const int multiplicand, const int multiplier) {
// 	ComputationResult computationResult = {
// 		.succeed = true,
// 		.value = multiplicand * multiplier
// 	};
// 	return computationResult;
// }

// ComputationResult subtract(const int minuend, const int subtract) {
// 	ComputationResult computationResult = {
// 		.succeed = true,
// 		.value = minuend - subtract
// 	};
// 	return computationResult;
// }

// ComputationResult computeConstant(Constant * constant) {
// 	ComputationResult computationResult = {
// 		.succeed = true,
// 		.value = constant->value
// 	};
// 	return computationResult;
// }

// ComputationResult computeExpression(Expression * expression) { // 2 + 3
// 	switch (expression->type) {
// 		case ADDITION:
// 		case DIVISION:
// 		case MULTIPLICATION:
// 		case SUBTRACTION:
// 			ComputationResult leftResult = computeExpression(expression->leftExpression);
// 			ComputationResult rightResult = computeExpression(expression->rightExpression);
// 			if (leftResult.succeed && rightResult.succeed) {
// 				BinaryOperator binaryOperator = _expressionTypeToBinaryOperator(expression->type);
// 				return binaryOperator(leftResult.value, rightResult.value);
// 			}
// 			else {
// 				return _invalidComputation();
// 			}
// 		case FACTOR:
// 			return computeFactor(expression->factor);
// 		default:
// 			return _invalidComputation();
// 	}
// }

// ComputationResult computeFactor(Factor * factor) {
// 	switch (factor->type) {
// 		case CONSTANT:
// 			return computeConstant(factor->constant);
// 		case EXPRESSION:
// 			return computeExpression(factor->expression);
// 		default:
// 			return _invalidComputation();
// 	}
// }
