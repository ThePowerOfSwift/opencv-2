import cv

if __name__ == '__main__':
  img = cv.LoadImageM ("cloud.jpg")
  cv.NamedWindow ("ShowImage")
  cv.ShowImage ("ShowImage", img)
  cv.WaitKey (0)
