#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;


// int main(int, char**) {
//     std::cout << "Hello, world!\n";
//     VideoCapture cap(0);
//     Mat img;

//     while (true) {
//         cap.read(img);
//         imshow("Image", img);
//         waitKey(20);
//     }
// }

 
// /////////////////  Images  //////////////////////
 
// void main() {
 
// 	string path = "Resources/test.png";
// 	Mat img = imread(path);
// 	imshow("Image", img);
// 	waitKey(0);
 
// }
 
 
///////////////  Video  //////////////////////
 
//void main() {
//
//	string path = "Resources/test_video.mp4";
//	VideoCapture cap(path);
//	Mat img;
//
//	while (true) {
//
//		cap.read(img);
//		imshow("Image", img);
//		waitKey(20);
//	}
//}
 
 
/////////////////  Webcam  //////////////////////
 


void getContours(Mat imgDil, Mat img) {

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(img, contours, -1, Scalar(255, 0, 255), 2);

	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());
	 
	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		string objectType;

		if (area > 1000) 
		{

            float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
			cout << conPoly[i].size() << endl;
			boundRect[i] = boundingRect(conPoly[i]);
		
			int objCor = (int)conPoly[i].size();

			if (objCor == 3) { objectType = "Tri"; }
			else if (objCor == 4)
			{ 
				float aspRatio = (float)boundRect[i].width / (float)boundRect[i].height;
				cout << aspRatio << endl;
				if (aspRatio> 0.95 && aspRatio< 1.05){ objectType = "Square"; }
				else { objectType = "Rect";}
			    drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
                // Cari titik Pusat Contour
                int cx = boundRect[i].x+boundRect[i].width/2; int cy = boundRect[i].y+boundRect[i].height/2; 
                // string centre = "cx" + "cy";
                string s1 = to_string(cx);
                string s2 = to_string(cy);
                string s3 = ", ";
                // Concatenate both strings
                string center = s1 + s3 +s2;
                putText(img, center , { cx, cy }, FONT_HERSHEY_PLAIN,1, Scalar(0, 69, 255), 2);
			}
			else if (objCor > 4) { objectType = "Circle"; }

			putText(img, objectType, { boundRect[i].x,boundRect[i].y - 5 }, FONT_HERSHEY_PLAIN,1, Scalar(0, 69, 255), 2);
			
            // float peri = arcLength(contours[i], true);
			// approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
			// boundRect[i] = boundingRect(conPoly[i]);
			// // cout << conPoly[i].size() << endl;
		
			// int objCor = (int)conPoly[i].size();

			// if (objCor = 4)
			// { 
            //     cout << objCor << "=4" << endl;
			// 	float aspRatio = (float)boundRect[i].width / (float)boundRect[i].height;
			// 	if (aspRatio> 0.95 && aspRatio< 1.05){ objectType = "Square"; }
			// 	else { objectType = "Rect";}


            //     drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
            //     // putText(img, objectType, { boundRect[i].x,boundRect[i].y - 5 }, FONT_HERSHEY_PLAIN,1, Scalar(0, 69, 255), 2);
            
            //     // // Cari titik Pusat Contour
            //     // int cx = boundRect[i].x+boundRect[i].width/2; int cy = boundRect[i].y+boundRect[i].height/2; 
            //     // // string centre = "cx" + "cy";
            //     // string s1 = to_string(cx);
            //     // string s2 = to_string(cy);
            //     // string s3 = ", ";
            //     // // Concatenate both strings
            //     // string center = s1 + s3 +s2;
            //     // putText(img, center , { cx, cy }, FONT_HERSHEY_PLAIN,1, Scalar(0, 69, 255), 2);
        
			// }
            // else if (objCor > 4) { objectType = "Circle"; cout << objCor << endl; }

		}
	}
}


void main() {

	VideoCapture cap(0);
	Mat img, HSV, mask, imgDil, imgErode;

	int hmin = 77, smin = 69, vmin = 108;
	int hmax = 115, smax = 177, vmax = 255;

	while (true) {
		cap.read(img);
        cvtColor(img, HSV, COLOR_RGB2HSV); //Convert BGR image to HSV image
        Scalar lower(hmin, smin, vmin);
		Scalar upper(hmax, smax, vmax);
		inRange(HSV, lower, upper, mask);


        Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
        erode(mask, imgErode, kernel);
        dilate(imgErode, imgDil, kernel);


        getContours(imgDil,img);    

		imshow("Image", img);
        imshow("Image Mask", mask);
        imshow("Image Erode", imgErode);


        //wait 20 ms between successive frames and break the loop if key q is pressed
        int key = waitKey(1);
        if (key == 'q')
        {
        break;
        }

	}
}

