import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from PIL import Image
from image_ops import grayscale_image, save_image, open_image

def test_grayscale_image():
    img = open_image("tests/input/prueba.png")
    gray = grayscale_image(img)
    os.makedirs("tests/output", exist_ok=True)
    save_image(gray, "tests/output/grayscale_image_result.png")
    print("test_grayscale_image: OK")

if __name__ == "__main__":
    test_grayscale_image()
