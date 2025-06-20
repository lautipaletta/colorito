import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '../../main/python')))
from PIL import Image
from image_ops import merge_image, save_image, open_image, resize_image

def test_merge_image():
    img1 = open_image("test/python/input/prueba.png")
    img2 = open_image("test/python/input/prueba2.png")
    img2 = resize_image(img2, (img1.width, img1.height))
    merged = merge_image(img1, img2, 'horizontal')
    os.makedirs("test/python/output", exist_ok=True)
    save_image(merged, "test/python/output/merge_image_result.png")
    print("test_merge_image: OK")

if __name__ == "__main__":
    test_merge_image()
