import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from PIL import Image
from image_ops import pixelate_image, save_image, open_image

def test_pixelate_image():
    img = open_image("tests/input/prueba.png")
    pixelated = pixelate_image(img, 10)
    os.makedirs("tests/output", exist_ok=True)
    save_image(pixelated, "tests/output/pixelate_image_result.png")
    print("test_pixelate_image: OK")

if __name__ == "__main__":
    test_pixelate_image()
