import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../../main/python')))
from PIL import Image
from image_ops import blend_image, save_image, open_image, resize_image

def test_blend_image():
    img1 = open_image("test/python/input/prueba.png")
    img2 = open_image("test/python/input/prueba2.png")
    img2 = resize_image(img2, (img1.width, img1.height))
    blended = blend_image(img1, img2, 0.5)
    os.makedirs("test/python/output", exist_ok=True)
    save_image(blended, "test/python/output/blend_image_result.png")
    print("test_blend_image: OK")

if __name__ == "__main__":
    test_blend_image()
