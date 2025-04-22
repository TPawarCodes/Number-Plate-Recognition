from flask import Flask, request, jsonify
import cv2
import numpy as np
import pytesseract

app = Flask(__name__)

@app.route('/ocr', methods=['POST'])
def ocr():
    try:
        xmin = int(request.args.get("xmin", 0))
        ymin = int(request.args.get("ymin", 0))
        xmax = int(request.args.get("xmax", 0))
        ymax = int(request.args.get("ymax", 0))
    except Exception as e:
        return jsonify({"error": "Invalid cropping parameters"}), 400

    img_bytes = request.data
    img = cv2.imdecode(np.frombuffer(img_bytes, np.uint8), cv2.IMREAD_COLOR)
    
    if xmax > xmin and ymax > ymin:
        img = img[ymin:ymax, xmin:xmax]
    
    # Preprocess: convert to grayscale and threshold
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    thresh = cv2.threshold(gray, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)[1]
    
    # OCR with Tesseract using custom configuration
    custom_config = r'--oem 3 --psm 6 -c tessedit_char_whitelist=ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789'
    text = pytesseract.image_to_string(thresh, config=custom_config)
    
    return jsonify({"NPR Data": text.strip()})

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
