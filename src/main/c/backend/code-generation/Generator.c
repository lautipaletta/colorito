#include "Generator.h"

/* MODULE INTERNAL STATE */

const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger * _logger = NULL;

void initializeGeneratorModule() {
	_logger = createLogger("Generator");
}

void shutdownGeneratorModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PRIVATE FUNCTIONS */

static const char _expressionTypeToCharacter(const ExpressionType type);
static void _generateConstant(const unsigned int indentationLevel, Constant * constant);
static void _generateEpilogue(const int value);
static void _generateExpression(const unsigned int indentationLevel, Expression * expression);
static void _generateFactor(const unsigned int indentationLevel, Factor * factor);
static void _generateProgram(Program * program);
static void _generatePrologue(void);
static char * _indentation(const unsigned int indentationLevel);
static void _output(const unsigned int indentationLevel, const char * const format, ...);

/**
 * Converts and expression type to the proper character of the operation
 * involved, or returns '\0' if that's not possible.
 */
static const char _expressionTypeToCharacter(const ExpressionType type) {
	switch (type) {
		case ADDITION: return '+';
		case DIVISION: return '/';
		case MULTIPLICATION: return '*';
		case SUBTRACTION: return '-';
		default:
			logError(_logger, "The specified expression type cannot be converted into character: %d", type);
			return '\0';
	}
}

/**
 * Generates the output of a constant.
 */
static void _generateConstant(const unsigned int indentationLevel, Constant * constant) {
	_output(indentationLevel, "%s", "[ $C$, circle, draw, black!20\n");
	_output(1 + indentationLevel, "%s%d%s", "[ $", constant->value, "$, circle, draw ]\n");
	_output(indentationLevel, "%s", "]\n");
}

/**
 * Creates the epilogue of the generated output, that is, the final lines that
 * completes a valid Latex document.
 */
static void _generateEpilogue(const int value) {
	_output(0, "%s%d%s",
		"            [ $", value, "$, circle, draw, blue ]\n"
		"        ]\n"
		"    \\end{forest}\n"
		"\\end{document}\n\n"
	);
}

/**
 * Generates the output of an expression.
 */
static void _generateExpression(const unsigned int indentationLevel, Expression * expression) {
	_output(indentationLevel, "%s", "[ $E$, circle, draw, black!20\n");
	switch (expression->type) {
		case ADDITION:
		case DIVISION:
		case MULTIPLICATION:
		case SUBTRACTION:
			_generateExpression(1 + indentationLevel, expression->leftExpression);
			_output(1 + indentationLevel, "%s%c%s", "[ $", _expressionTypeToCharacter(expression->type), "$, circle, draw, purple ]\n");
			_generateExpression(1 + indentationLevel, expression->rightExpression);
			break;
		case FACTOR:
			_generateFactor(1 + indentationLevel, expression->factor);
			break;
		default:
			logError(_logger, "The specified expression type is unknown: %d", expression->type);
			break;
	}
	_output(indentationLevel, "%s", "]\n");
}

/**
 * Generates the output of a factor.
 */
static void _generateFactor(const unsigned int indentationLevel, Factor * factor) {
	_output(indentationLevel, "%s", "[ $F$, circle, draw, black!20\n");
	switch (factor->type) {
		case CONSTANT:
			_generateConstant(1 + indentationLevel, factor->constant);
			break;
		case EXPRESSION:
			_output(1 + indentationLevel, "%s", "[ $($, circle, draw, purple ]\n");
			_generateExpression(1 + indentationLevel, factor->expression);
			_output(1 + indentationLevel, "%s", "[ $)$, circle, draw, purple ]\n");
			break;
		default:
			logError(_logger, "The specified factor type is unknown: %d", factor->type);
			break;
	}
	_output(indentationLevel, "%s", "]\n");
}

/**
 * Generates the output of the program.
 */
static void _generateProgram(Program * program) {
	_generateExpression(3, program->expression);
}

/**
 * Creates the prologue of the generated output, a Latex document that renders
 * a tree thanks to the Forest package.
 *
 * @see https://ctan.dcc.uchile.cl/graphics/pgf/contrib/forest/forest-doc.pdf
 */
static void _generatePrologue(void) {
	_output(0, "%s",
		"\\documentclass{standalone}\n\n"
		"\\usepackage[utf8]{inputenc}\n"
		"\\usepackage[T1]{fontenc}\n"
		"\\usepackage{amsmath}\n"
		"\\usepackage{forest}\n"
		"\\usepackage{microtype}\n\n"
		"\\begin{document}\n"
		"    \\centering\n"
		"    \\begin{forest}\n"
		"        [ \\text{$=$}, circle, draw, purple\n"
	);
}

/**
 * Generates an indentation string for the specified level.
 */
static char * _indentation(const unsigned int level) {
	return indentation(_indentationCharacter, level, _indentationSize);
}

/**
 * Outputs a formatted string to standard output. The "fflush" instruction
 * allows to see the output even close to a failure, because it drops the
 * buffering.
 */
static void _output(const unsigned int indentationLevel, const char * const format, ...) {
	va_list arguments;
	va_start(arguments, format);
	char * indentation = _indentation(indentationLevel);
	char * effectiveFormat = concatenate(2, indentation, format);
	vfprintf(stdout, effectiveFormat, arguments);
	fflush(stdout);
	free(effectiveFormat);
	free(indentation);
	va_end(arguments);
}

/** PUBLIC FUNCTIONS */

void generate(CompilerState * compilerState) {
	logDebugging(_logger, "Generating final output...");
	_generatePrologue();
	_generateProgram(compilerState->abstractSyntaxtTree);
	_generateEpilogue(compilerState->value);
	logDebugging(_logger, "Generation is done.");
}

// **********************idea*********************************
/* === SYMBOL TABLE === */

typedef struct VariableEntry {
	const char * name;
	VariableType type;
	struct VariableEntry * next;
} VariableEntry;

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

static bool _checkVariableWithType(Variable * var, VariableType expectedType) {
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

static bool _checkFactor(Factor * factor);
static bool _checkExpression(Expression * expression);

static bool _checkFactor(Factor * factor) {
	if (!factor) return false;
	if (factor->type == FACTOR_EXPRESSION) {
		return _checkExpression(factor->data.expression);
	} else if (factor->type == FACTOR_IDENTIFIER) {
		VariableEntry * entry = _findVariable(factor->data.identifier);
		if (!entry) {
			logError(_logger, "Identificador no declarado: %s", factor->data.identifier);
			return false;
		}
	}
	return true;
}

static bool _checkExpression(Expression * expr) {
	if (!expr) return false;

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
			       _checkVariableWithType(expr->data.dual_op.param.blend_factor, INTEGER_TYPE);

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

static bool _checkLine(Line * line) {
	if (!line) return true;
	bool result = true;

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

bool analyzeSemantics(CompilerState * compilerState) {
	logDebugging(_logger, "Iniciando análisis semántico...");
	_clearSymbolTable();
	bool success = compilerState->abstractSyntaxtTree != NULL &&
	               _checkLine(compilerState->abstractSyntaxtTree->line);
	if (!success) {
		logError(_logger, "Análisis semántico fallido.");
		compilerState->succeed = false;
	} else {
		logDebugging(_logger, "Análisis semántico finalizado exitosamente.");
		compilerState->succeed = true;
	}
	_clearSymbolTable();
	return success;
}