# Dropping Zone detection 
# Program ini melakukan masking kepada warna tertentu (bisa diatur pakai slider)
# Kemudian melakukan deteksi contour dan bentuk berdasarkan jumlah contour
# Kemudian Mendapatkan koordinat titik contour

import cv2
import numpy as np

# WEBCAM

cap = cv2.VideoCapture(0) # 0 is for webcam
# cap = cv2.VideoCapture(r"assets/DZ_ums.mp4") # 0 is for webcam


def nothing(x):
    pass

#Window untuk trackbar
cv2.namedWindow("Trackbar")
cv2.createTrackbar("Low_Hue", "Trackbar", 0, 180, nothing)
cv2.createTrackbar("Low_Saturation", "Trackbar", 0, 255, nothing)
cv2.createTrackbar("Low_Value", "Trackbar", 0, 255, nothing)
cv2.createTrackbar("Upper_Hue", "Trackbar", 180, 180, nothing)
cv2.createTrackbar("Upper_Saturation", "Trackbar", 255, 255, nothing)
cv2.createTrackbar("Upper_Value", "Trackbar", 255, 255, nothing)

font = cv2.FONT_HERSHEY_COMPLEX

while True:
    success, img = cap.read()

    # # Kalau mau cb pict
    # img = cv2.imread("assets/DZ_pict6.jpeg")


    #processing
    hsv = cv2.cvtColor(img, cv2.COLOR_RGB2HSV)

    #getting the limits
    Low_Hue = cv2.getTrackbarPos("Low_Hue", "Trackbar")
    Low_Saturation = cv2.getTrackbarPos("Low_Saturation", "Trackbar")
    Low_Value = cv2.getTrackbarPos("Low_Value", "Trackbar")
    Upper_Hue = cv2.getTrackbarPos("Upper_Hue", "Trackbar")
    Upper_Saturation = cv2.getTrackbarPos("Upper_Saturation", "Trackbar")
    Upper_Value = cv2.getTrackbarPos("Upper_Value", "Trackbar")


    #batas
    lower = np.array([Low_Hue, Low_Saturation, Low_Value])
    upper = np.array([Upper_Hue, Upper_Saturation, Upper_Value])

    #masking
    mask = cv2.inRange(hsv, lower, upper)
    kernel = np.ones((5,5), np.uint8)
    mask = cv2.erode(mask, kernel) #erosion untuk mengurangi noise
    


    # Contour detection
    contours, hierarchy = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    for cnt in contours:
        area = cv2.contourArea(cnt)
        approx = cv2.approxPolyDP(cnt, 0.01*cv2.arcLength(cnt, True), True)


        if area > 400:
            cv2.drawContours(img, [approx], 0, (0,0,0), 3)
            if len(approx) == 4:
                print("Rectangle")


                ##########################
                # Bagian nentuin pixel coordinate dari rectangle
                # top left pixel
                x1 = approx.ravel()[0]
                y1 = approx.ravel()[1]

                # top right pixel
                x2 = approx.ravel()[4]
                y2 = approx.ravel()[5]

                # bottom left pixel
                x3 = approx.ravel()[2]
                y3 = approx.ravel()[3]

                # bottom right pixel
                x4 = approx.ravel()[6]
                y4 = approx.ravel()[7]
                # print(approx)
                # print(x1, y1)


                #print pixel ke layar
                cv2.putText(img, f"({x1}, {y1}, top_left", (x1, y1), font, 1, (0,0,0))  
                cv2.putText(img, f"({x2}, {y2},top_right", (x2, y2), font, 1, (0,0,0))  
                cv2.putText(img, f"({x3}, {y3},bottom_left", (x3, y3), font, 1, (0,0,0))  
                cv2.putText(img, f"({x4}, {y4},bottom_right", (x4, y4), font, 1, (0,0,0))


                # Deteksi center dari contour
                M = cv2.moments(cnt)
                cX = int(M["m10"] / M["m00"])
                cY = int(M["m01"] / M["m00"])
                
                # draw the contour and center of the shape on the image
                # cv2.drawContours(img, [cnt], -1, (0, 255, 0), 2)
                cv2.circle(img, (cX, cY), 7, (255, 255, 255), -1)
                cv2.putText(img, f"{cX}, {cY}, center", (cX - 20, cY - 20),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 0, 255), 2)



    #results
    cv2.imshow('mask', mask)
    cv2.imshow('img', img)

    if cv2.waitKey(1) & 0xFF ==ord('q'):  #to make exit key (necesarry)
        break
