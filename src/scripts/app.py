from io import BytesIO
from fastapi.responses import JSONResponse
from pipeline import resize_image, model_prediction
from fastapi import FastAPI, File, Form, UploadFile


app = FastAPI()

@app.get("/")
async def test():
    return {"Server Alive": "Connected"}

@app.post("/upload_image")
async def get_image(
    file: UploadFile = File(...)
):
    image = await file.read()
    bytes_io = BytesIO(image)
    image_file = open("buffer.jpg", "wb")
    image_file.write(bytes_io.read())
    image_file.close()
    bytes_io.close()
    
    res = resize_image("buffer.jpg")
    if (res.any()):
        prediction = model_prediction(res)
        response = f"The image inserted prediction is: {prediction}."
        return response
    
    else:
        response = "Error: image cannot be loaded! please input a valid image."
        return response