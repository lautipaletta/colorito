import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from PIL import Image
from image_ops import resize_image, save_image, open_image

def test_resize_image():
    img = open_image("tests/input/prueba.png")
    resized = resize_image(img, (50, 200))
    os.makedirs("tests/output", exist_ok=True)
    save_image(resized, "tests/output/resize_image_result.png")
    print("test_resize_image: OK")

if __name__ == "__main__":
    test_resize_image()
