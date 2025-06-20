import os
import sys
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../../main/python')))
from PIL import Image
from image_ops import recolor_image, save_image, open_image

def test_recolor_image():
    img = open_image("test/python/input/prueba2.jpg")
    recolored = recolor_image(img, "#e0e000", "#ffff80", "#0000ff")
    os.makedirs("test/python/output", exist_ok=True)
    save_image(recolored, "test/python/output/recolor_image_result.jpg")
    print("test_recolor_image: OK")

if __name__ == "__main__":
    test_recolor_image()
