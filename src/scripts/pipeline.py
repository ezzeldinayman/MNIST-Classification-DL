import cv2
import numpy as np
import pickle as pkl
import tensorflow as tf


model = tf.keras.models.load_model("deployment_model.h5")

def resize_image(image_path, target_shape=(28,28,1)):
    
    image = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)


    # Resize the image to the target shape
    resized_image = cv2.resize(image, (target_shape[1], target_shape[0]))
    
    
    # Add an additional dimension to the image
    processed_image = np.expand_dims(resized_image, axis=0)
    
    return processed_image

def model_prediction(image):
    
    y_vector = model.predict(image)
    return np.argmax(y_vector, axis=1)
    