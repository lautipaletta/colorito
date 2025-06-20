import sys
import logging
import traceback

# Setup logging
logging.basicConfig(
    level=logging.INFO,
    format='[%(asctime)s] %(levelname)s: %(message)s',
    handlers=[logging.StreamHandler(sys.stdout)]
)

logger = logging.getLogger(__name__)


def run_generated_code(code_path):
    """
    Executes Python code generated from a file.
    """
    try:
        with open(code_path, 'r', encoding='utf-8') as f:
            code = f.read()
        logger.info(f"Executing generated code from: {code_path}")
        exec_globals = {
            '__builtins__': __builtins__,
        }
        exec(code, exec_globals)
        logger.info("Execution completed successfully.")
    except Exception as e:
        logger.error(f"Error during execution: {e}")
        traceback.print_exc()


def main():
    if len(sys.argv) != 2:
        print("Usage: python runtime.py <generated_code.py>")
        sys.exit(1)
    code_path = sys.argv[1]
    run_generated_code(code_path)

if __name__ == "__main__":
    main()
