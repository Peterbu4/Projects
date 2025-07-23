from ultralytics import YOLO
import cv2

# Loading pretrained model (YOLOv8)
model = YOLO("yolov8n.pt")

# read the image
image_path = "example.png"
image = cv2.imread(image_path)

# Objects detection
results = model(image)

# Processing and marking detected objects
for result in results:
    for box in result.boxes:
        x1, y1, x2, y2 = map(int, box.xyxy[0])  # Coordinates
        label = f"{model.names[int(box.cls)]}: {box.conf[0]:.2f}"  # name + certainty
        cv2.rectangle(image, (x1, y1), (x2, y2), (0, 255, 0), 2)
        cv2.putText(image, label, (x1 + 5, y2 - 5), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

# Showing the results
cv2.imshow("Detekcja obiektów", image)
cv2.waitKey(0)
cv2.destroyAllWindows()
