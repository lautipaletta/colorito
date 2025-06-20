import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from PIL import Image
from image_ops import sharpen_image, save_image, open_image

def test_sharpen_image():
    img = open_image("tests/input/prueba.png")
    sharpened = sharpen_image(img)
    os.makedirs("tests/output", exist_ok=True)
    save_image(sharpened, "tests/output/sharpen_image_result.png")
    print("test_sharpen_image: OK")

if __name__ == "__main__":
    test_sharpen_image()
