import cv2

if __name__ == '__main__':
  img = cv2.imread ("cloud.jpg")
  cv2.namedWindow ("ShowImage")
  cv2.imshow ("ShowImage", img)
  cv2.waitKey (0)
  cv2.destroyAllWindows()
