import os
import sys
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from PIL import Image
from image_ops import recolor_image, save_image, open_image

def test_recolor_image():
    img = open_image("tests/input/prueba2.png")
    recolored = recolor_image(img, "#e0e000", "#ffff80", "#0000ff")
    os.makedirs("tests/output", exist_ok=True)
    save_image(recolored, "tests/output/recolor_image_result.png")
    print("test_recolor_image: OK")

if __name__ == "__main__":
    test_recolor_image()
