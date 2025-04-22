# Real-Time License Plate Detection and Recognition on ESP32-CAM
## Project Overview
This project shows a real-time license plate detection and recognition system implemented on an ESP32-CAM module. The system employs a MobileNetV2 SSD model for license plate detection and incorporates Tesseract OCR for text recognition. Edge processing is supported using TFLite and int8 quantization, which ensures low-latency and efficient execution on hardware with limited resources.

## Objectives
Train and deploy a MobileNetV2 SSD model for license plate detection.

Quantize the model to TFLite format with int8 for edge deployment.

Implement Tesseract OCR to recognize characters on the license plates.

Deploy the solution on an ESP32-CAM with the Arduino IDE.

## Methodology
### 2.1 Data Preparation and Collection
Dataset: Annotated car images with license plates in Pascal VOC format.

Preprocessing: Resize images to 300x300; perform augmentation (flip, rotate, brightness).

### ️ 2.2 Model Structure
MobileNetV2 SSD: Optimized and lightweight for edge devices.

SSD Framework: Efficient object detection using default bounding boxes.

###  2.3 Training Process
Framework: TensorFlow 2.x with GPU acceleration.

Hyperparameters:

Learning Rate: 0.001

Batch Size: 16

Epochs: 50

Optimizer: Adam

Checkpointing: Save weights at regular intervals.

###  2.4 Model Conversion & Quantization
Conversion: Utilize TFLiteConverter to convert the model into.tflite.

Quantization: Perform post-training int8 quantization for optimization.

###  2.5 ESP32-CAM Deployment
Platform: Arduino IDE.

Process: Upload detect.tflite model with the esp_32_upload_code.ino and utilize onboard camera for real-time inference.

###  2.6 Tesseract OCR
Tesseract Server: Run on a local system.

Integration: ESP32-CAM passes cropped license plate images to server for OCR.

