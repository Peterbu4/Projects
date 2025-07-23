import cv2
import numpy as np
import matplotlib.pyplot as plt

def to_gray(image):
    if len(image.shape) == 2:
        return image
    return cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

def rotate(image, angle):
    (h, w) = image.shape[:2]
    center = (w // 2, h // 2)
    matrix = cv2.getRotationMatrix2D(center, angle, 1.0)
    return cv2.warpAffine(image, matrix, (w, h))

def scale(image, factor):
    h, w = image.shape[:2]
    print(f"Before scaling: width={w}, hight={h}")
    if factor > 1:
        interpolation = cv2.INTER_CUBIC
    else:
        interpolation = cv2.INTER_AREA
    scaled = cv2.resize(image, None, fx=factor, fy=factor, interpolation=interpolation)
    h2, w2 = scaled.shape[:2]
    print(f"After scaling: width={w2}, hight={h2}")
    return scaled

def detect_edges(image):
    return cv2.Canny(image, 100, 200)

def sharpen(image):
    # Kernel for sharpening
    kernel = np.array([[0, -1, 0],
                       [-1, 5,-1],
                       [0, -1, 0]])
    return cv2.filter2D(image, -1, kernel)

def show_image_matplotlib(image, title="Obraz"):
    if len(image.shape) == 2:
        plt.imshow(image, cmap="gray")
    else:
        image_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        plt.imshow(image_rgb)
    plt.title(title)
    plt.axis("off")
    plt.show()

def main():

    image = cv2.imread("path_to_file.png") # Provided that image is in the same directory as the running program

    if image is None:
        print("Faild to load image!")
        return

    original_image = image.copy()
    show_image_matplotlib(image, "Image after processing")

    while True:
        print("\nChoose option:")
        print("1 - Conversion to grayscale")
        print("2 - Rotation")
        print("3 - Scaling")
        print("4 - Edge detection")
        print("5 - Sharpening")
        print("6 - Reset changes and show original image")
        print("7 - Display the image")
        print("0 - Exit")
        choice = input("Your choice: ")

        if choice == "1":
            image = to_gray(image)
            show_image_matplotlib(image, "Image after conversion to grayscale")
        elif choice == "2":
            angle = float(input("Enter the rotation angle (positive value = left, negative value = right): "))
            image = rotate(image, angle)
            show_image_matplotlib(image, "Image after rotation")
        elif choice == "3":
            factor = float(input("Enter scaling factor (for instance 1.5 means enlarging the image by 1,5x): "))
            image = scale(image, factor)
            show_image_matplotlib(image, "Image after rescalinig")
        elif choice == "4":
            gray_image = to_gray(image)
            image = detect_edges(gray_image)
            show_image_matplotlib(image, "Image after edges detection")
        elif choice == "5":
            image = sharpen(image)
            show_image_matplotlib(image, "Image after sharpening")
        elif choice == "6":
            image = original_image.copy()
            show_image_matplotlib(image, "Restored original image")
        elif choice == "7":
            show_image_matplotlib(image, "Image after processing")
        elif choice == "0":
            break
        else:
            print("Incorrect choice!")
            continue

    show_image_matplotlib(image, "Image after processing") # showing the final result before ending the program

if __name__ == "__main__":
    main()
