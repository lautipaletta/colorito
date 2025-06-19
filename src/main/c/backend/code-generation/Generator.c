#include "Generator.h"

/* MODULE INTERNAL STATE */

const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger * _logger = NULL;
static FILE * _outputFile = NULL;

void initializeGeneratorModule() {
	_logger = createLogger("Generator");
}

// void shutdownGeneratorModule() {
// 	if (_logger != NULL) {
// 		destroyLogger(_logger);
// 	}
// }

// /** PRIVATE FUNCTIONS */

// static void _generateEpilogue(const int value);
// static void _generatePrologue(void);

static void _generateProgram(Program * program);
static void _generateLines(const unsigned int indentationLevel, Line * line);
static void _generateVariableDeclaration(const unsigned int indentationLevel, const char * identifier, Variable * variable);
static void _generateExpression(const unsigned int indentationLevel, Expression * expression);
static void _generateFactor(const unsigned int indentationLevel, Factor * factor);
static void _generateVariable(const unsigned int indentationLevel, Variable * variable);

static char * _indentation(const unsigned int indentationLevel);
static void _output(const unsigned int indentationLevel, const char * const format, ...);

static void _generateCropExpression(const unsigned int indentationLevel, Expression * expression);
static void _generateOpenExpression(const unsigned int indentationLevel, Expression * expression);
static void _generateSaveExpression(const unsigned int indentationLevel, Expression * expression);
static void _generateResizeExpression(const unsigned int indentationLevel, Expression * expression);
static void _generateRotateExpression(const unsigned int indentationLevel, Expression * expression);
static void _generateBrightnessExpression(const unsigned int indentationLevel, Expression * expression);
static void _generateContrastExpression(const unsigned int indentationLevel, Expression * expression);
static void _generateBlurExpression(const unsigned int indentationLevel, Expression * expression);
static void _generatePixelateExpression(const unsigned int indentationLevel, Expression * expression);
static void _generateOpacityExpression(const unsigned int indentationLevel, Expression * expression);
static void _generateFlipExpression(const unsigned int indentationLevel, Expression * expression);
static void _generateGrayscaleExpression(const unsigned int indentationLevel, Expression * expression);
static void _generateInvertExpression(const unsigned int indentationLevel, Expression * expression);
static void _generateSharpenExpression(const unsigned int indentationLevel, Expression * expression);
static void _generateBlendImagesExpression(const unsigned int indentationLevel, Expression * expression);
static void _generateMergeImagesExpression(const unsigned int indentationLevel, Expression * expression);
static void _generateRecolorImageExpression(const unsigned int indentationLevel, Expression * expression);

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
	vfprintf(_outputFile, effectiveFormat, arguments);
	fflush(_outputFile);
	free(effectiveFormat);
	free(indentation);
	va_end(arguments);
}

static void _generateProgram(Program * program) {
	_generateLines(0, program->line);
}


/**
 * Generates the output of the lines in the program.
 * It can be an expression or a variable declaration.
 */
static void _generateLines(const unsigned int indentationLevel, Line * line) {
    while (line) {
        switch (line->type) {
            case LINE_EXPRESSION:
                _generateExpression(indentationLevel, line->content.expression);
                _output(0, ";\n");  
                break;
            case LINE_VARIABLE_DECLARATION:
                _generateVariableDeclaration(indentationLevel, line->content.variable_declaration.identifier, line->content.variable_declaration.variable);
                _output(0, "\n");
                break;
        }
        line = line->next;
    }
}

/**
 * Generates the output of a variable declaration.
 */
static void _generateVariableDeclaration(const unsigned int indentationLevel, const char * identifier, Variable * variable) {
    // Output the variable declaration
    switch (variable->type) {
        case STRING_TYPE:
            _output(indentationLevel, "%s %s = ", "String", identifier);
            _output(0, "\"%s\";", variable->data.string);
            break;
        case INTEGER_TYPE:
            _output(indentationLevel, "%s %s = ", "int", identifier);
            _output(0, "%d;", variable->data.integer);
            break;
        case EXPRESSION_TYPE:
            _output(indentationLevel, "%s %s = ", "Expression", identifier);        
            _generateExpression(indentationLevel, variable->data.expression);
            _output(0, ";");
            break;
        case COLOR_TYPE:
            _output(indentationLevel, "%s %s = ", "Color", identifier);
            _output(0, "color(%s);", variable->data.color);
            break;
        case PERCENTAGE_TYPE:
            _output(indentationLevel, "%s %s = ", "Percentage", identifier);
            _output(0, "%d%%;", variable->data.percentage);
            break;
        case IDENTIFIER_TYPE:
            _output(indentationLevel, "%s %s = ", "Identifier", identifier);
            _output(0, "%s;", variable->data.identifier);
            break;
        case DIMENSION_TYPE:
            _output(indentationLevel, "%s %s = ", "Dimension", identifier);
            _output(0, "%s;", variable->data.dimension);
            break;
        default:
            logError(_logger, "Unknown variable type: %d", variable->type);
    }
}

static void _generateFactor(const unsigned int indentationLevel, Factor * factor) {
    // Output the factor
    switch (factor->type) {
        case FACTOR_EXPRESSION:
            _output(indentationLevel, "( ");
            _generateExpression(indentationLevel, factor->data.expression);
            _output(indentationLevel, " )");
            break;
        case FACTOR_IDENTIFIER:
            _output(indentationLevel, "%s", factor->data.identifier);
            break;
        default:
            logError(_logger, "Unknown factor type: %d", factor->type);
    }
}

static void _generateVariable(const unsigned int indentationLevel, Variable * variable) {
    // Output the variable
    switch (variable->type) {
        case STRING_TYPE:
            _output(indentationLevel, "\"%s\"", variable->data.string);
            break;
        case INTEGER_TYPE:
            _output(indentationLevel, "%d", variable->data.integer);
            break;
        case EXPRESSION_TYPE:
            _generateExpression(indentationLevel, variable->data.expression);
            break;
        case COLOR_TYPE:
            _output(indentationLevel, "color(%s)", variable->data.color);
            break;
        case PERCENTAGE_TYPE:
            _output(indentationLevel, "%d%%", variable->data.percentage);
            break;
        case IDENTIFIER_TYPE:
            _output(indentationLevel, "%s", variable->data.identifier);
            break;
        case DIMENSION_TYPE:
            _output(indentationLevel, "%s", variable->data.dimension);
            break;
        default:
            logError(_logger, "Unknown variable type: %d", variable->type);
    }
}

static void _generateExpression(const unsigned int indentationLevel, Expression * expression) {
    // Output the expression
    switch (expression->type) {
        case OPEN_IMAGE:
            _generateOpenExpression(indentationLevel, expression);
            break;
        case SAVE_IMAGE:
            _generateSaveExpression(indentationLevel, expression);
            break;
        case CROP_IMAGE:
            _generateCropExpression(indentationLevel, expression);
            break;
        case RESIZE_IMAGE:
            _generateResizeExpression(indentationLevel, expression);
            break;
        case ROTATE_IMAGE:
            _generateRotateExpression(indentationLevel, expression);
            break;
        case BRIGHTNESS_IMAGE:
            _generateBrightnessExpression(indentationLevel, expression);
            break;
        case CONTRAST_IMAGE:
            _generateContrastExpression(indentationLevel, expression);
            break;          
        case BLUR_IMAGE:
            _generateBlurExpression(indentationLevel, expression);
            break;
        case PIXELATE_IMAGE:
            _generatePixelateExpression(indentationLevel, expression);
            break;
        case OPACITY_IMAGE:
            _generateOpacityExpression(indentationLevel, expression);
            break;
        case FLIP_IMAGE:
            _generateFlipExpression(indentationLevel, expression);
            break;
        case GRAYSCALE_IMAGE:
            _generateGrayscaleExpression(indentationLevel, expression);
            break;
        case INVERT_IMAGE:      
            _generateInvertExpression(indentationLevel, expression);
            break;
        case SHARPEN_IMAGE:
            _generateSharpenExpression(indentationLevel, expression);
            break;
        case BLEND_IMAGES:
            _generateBlendImagesExpression(indentationLevel, expression);
            break;
        case MERGE_IMAGES:
            _generateMergeImagesExpression(indentationLevel, expression);
            break;
        case RECOLOR_IMAGE:
            _generateRecolorImageExpression(indentationLevel, expression);
            break;
        default:
            logError(_logger, "Unknown expression type: %d", expression->type);
    }
}

static void _generateCropExpression(const unsigned int indentationLevel, Expression * expression) {
    _output(indentationLevel, "crop_image(");
    _generateFactor(indentationLevel, expression->data.crop.image);
    _output(0, ", ");
    _generateVariable(indentationLevel, expression->data.crop.divisions_qty);
    _output(0, ", ");
    _generateVariable(indentationLevel, expression->data.crop.output_division);
    _output(0, ")");
}

static void _generateOpenExpression(const unsigned int indentationLevel, Expression * expression) {
    _output(indentationLevel, "open_image( ");
    _generateVariable(0, expression->data.open.filename);
    _output(0, " )");
}

static void _generateSaveExpression(const unsigned int indentationLevel, Expression * expression) {
    _output(indentationLevel, "save_image( ");
    _generateFactor(indentationLevel, expression->data.save.image);
    _output(0, ", ");
    _generateVariable(indentationLevel, expression->data.save.filename);
    _output(0, " )");
}

static void _generateResizeExpression(const unsigned int indentationLevel, Expression * expression) {
    _output(indentationLevel, "resize_image( ");
    _generateFactor(indentationLevel, expression->data.resize.image);
    _output(0, ", ");
    _generateVariable(indentationLevel, expression->data.resize.dimension);
    _output(0, " )");
}

static void _generateRotateExpression(const unsigned int indentationLevel, Expression * expression) {
    _output(indentationLevel, "rotate_image( ");
    _generateFactor(indentationLevel, expression->data.numeric_op.image);
    _output(0, ", ");
    _generateVariable(indentationLevel, expression->data.numeric_op.value);
    _output(0, " )");
}

static void _generateBrightnessExpression(const unsigned int indentationLevel, Expression * expression) {
    _output(indentationLevel, "brightness_image( ");
    _generateFactor(indentationLevel, expression->data.numeric_op.image);
    _output(0, ", ");
    _generateVariable(indentationLevel, expression->data.numeric_op.value);
    _output(0, " )");
}

static void _generateContrastExpression(const unsigned int indentationLevel, Expression * expression) {
    _output(indentationLevel, "contrast_image( ");
    _generateFactor(indentationLevel, expression->data.numeric_op.image);
    _output(0, ", ");
    _generateVariable(indentationLevel, expression->data.numeric_op.value);
    _output(0, " )");
}

static void _generateBlurExpression(const unsigned int indentationLevel, Expression * expression) {
    _output(indentationLevel, "blur_image( ");
    _generateFactor(indentationLevel, expression->data.numeric_op.image);
    _output(0, ", ");
    _generateVariable(indentationLevel, expression->data.numeric_op.value);
    _output(0, " )");
}

static void _generatePixelateExpression(const unsigned int indentationLevel, Expression * expression) {
    _output(indentationLevel, "pixelate_image( ");
    _generateFactor(indentationLevel, expression->data.numeric_op.image);
    _output(0, ", ");
    _generateVariable(indentationLevel, expression->data.numeric_op.value);
    _output(0, " )");
}

static void _generateOpacityExpression(const unsigned int indentationLevel, Expression * expression) {
    _output(indentationLevel, "opacity_image( ");
    _generateFactor(indentationLevel, expression->data.numeric_op.image);
    _output(0, ", ");
    _generateVariable(indentationLevel, expression->data.numeric_op.value);
    _output(0, " )");
}

static void _generateFlipExpression(const unsigned int indentationLevel, Expression * expression) {
    _output(indentationLevel, "flip_image( ");
    _generateFactor(indentationLevel, expression->data.directional_op.image);
    _output(0, ", ");
    switch (expression->data.directional_op.direction) {
        case HORIZONTAL:
            _output(0, "HORIZONTAL");
            break;
        case VERTICAL:
            _output(0, "VERTICAL");
            break;
        default:
            logError(_logger, "Unknown direction: %d", expression->data.directional_op.direction);
    }
    _output(0, " )");
}

static void _generateGrayscaleExpression(const unsigned int indentationLevel, Expression * expression) {
    _output(indentationLevel, "grayscale_image( ");
    _generateFactor(indentationLevel, expression->data.simple_op.image);
    _output(0, " )");
}

static void _generateInvertExpression(const unsigned int indentationLevel, Expression * expression) {
    _output(indentationLevel, "invert_image( ");
    _generateFactor(indentationLevel, expression->data.simple_op.image);
    _output(0, " )");
}

static void _generateSharpenExpression(const unsigned int indentationLevel, Expression * expression) {
    _output(indentationLevel, "sharpen_image( ");
    _generateFactor(indentationLevel, expression->data.simple_op.image);
    _output(0, " )");
}

static void _generateBlendImagesExpression(const unsigned int indentationLevel, Expression * expression) {
    _output(indentationLevel, "blend_images( ");
    _generateFactor(indentationLevel, expression->data.dual_op.image1);
    _output(0, ", ");
    _generateFactor(indentationLevel, expression->data.dual_op.image2);
    _output(0, ", ");
    _generateVariable(indentationLevel, expression->data.dual_op.param.blend_factor);
    _output(0, " )");
}

static void _generateMergeImagesExpression(const unsigned int indentationLevel, Expression * expression) {
    _output(indentationLevel, "merge_images( ");
    _generateFactor(indentationLevel, expression->data.dual_op.image1);
    _output(0, ", ");
    _generateFactor(indentationLevel, expression->data.dual_op.image2);
    _output(0, ", ");
    switch (expression->data.dual_op.param.direction) {
        case HORIZONTAL:
            _output(0, "HORIZONTAL");
            break;
        case VERTICAL:
            _output(0, "VERTICAL");
            break;
        default:
            logError(_logger, "Unknown direction: %d", expression->data.dual_op.param.direction);
    }
    _output(0, " )");
}

static void _generateRecolorImageExpression(const unsigned int indentationLevel, Expression * expression) {
    _output(indentationLevel, "recolor_image( ");
    _generateFactor(indentationLevel, expression->data.recolor.image);
    _output(0, ", ");
    _generateVariable(indentationLevel, expression->data.recolor.from_color1);
    _output(0, ", ");
    _generateVariable(indentationLevel, expression->data.recolor.from_color2);
    _output(0, ", ");
    _generateVariable(indentationLevel, expression->data.recolor.to_color);
    _output(0, " )");
}

/** PUBLIC FUNCTIONS */

void generate(CompilerState * compilerState) {
	logDebugging(_logger, "Generating final output...");

    // TODO: allow other file names to output
    _outputFile = fopen("output.txt", "w");
    if (!_outputFile) {
        logError(_logger, "No se pudo abrir el archivo de salida. Using stdout instead.");
        _outputFile = stdout;
    }

	// _generatePrologue();        // TODO: imports here
	_generateProgram(compilerState->abstractSyntaxtTree);
	// _generateEpilogue(compilerState->value); // value does not exist in the anymore it was for calculator
	logDebugging(_logger, "Generation is done.");
}
