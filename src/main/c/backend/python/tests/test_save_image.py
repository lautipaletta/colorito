import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from PIL import Image
from image_ops import save_image, open_image

def test_save_image():
    img = open_image("tests/input/prueba.png")
    os.makedirs("tests/output", exist_ok=True)
    save_path = "tests/output/save_image_result.png"
    save_image(img, save_path)
    assert os.path.exists(save_path)
    print("test_save_image: OK")

if __name__ == "__main__":
    test_save_image()
