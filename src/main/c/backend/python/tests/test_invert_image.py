import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from PIL import Image
from image_ops import invert_image, save_image, open_image

def test_invert_image():
    img = open_image("tests/input/prueba.png")
    inverted = invert_image(img)
    os.makedirs("tests/output", exist_ok=True)
    save_image(inverted, "tests/output/invert_image_result.png")
    print("test_invert_image: OK")

if __name__ == "__main__":
    test_invert_image()
