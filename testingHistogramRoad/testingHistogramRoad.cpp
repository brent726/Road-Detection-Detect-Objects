
#include "stdafx.h"
#include "highgui.h"
#include "cv.h"
#include "conio.h"
#include <vector>   
#include <windows.h>
#include <iostream>
using namespace cv;

using namespace std;

// Global variables
//Mat inputImage[5], outputImage, channel[3], labImage, roadImage,labRoadImage;
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {
          cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
     else if  ( event == EVENT_RBUTTONDOWN )
     {
          cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
     else if  ( event == EVENT_MBUTTONDOWN )
     {
          cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
    
}




int main(int argc, char** argv){
	// read colored BGR image

	Mat inputImage[5], outputImage, channel[3], labImage, roadImage,labRoadImage;
	int x,y,L,a,b;
	int xSize, ySize;
	Mat originalRoadImage;
	Mat roadGray;
	Mat roadBlurImage;

	Mat gradImage;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	roadImage=imread("1.jpg");
	
	if( !roadImage.data )
	{ return -1; }
   imshow( "Road Image", roadImage );
  GaussianBlur( roadImage, roadBlurImage, Size(3,3), 0, 0, BORDER_DEFAULT );
  /// Convert it to gray
  cvtColor( roadBlurImage, roadGray, CV_BGR2GRAY );

  /// Generate grad_x and grad_y
  Mat grad_x, grad_y;
  Mat abs_grad_x, abs_grad_y;

  /// Gradient X
  Sobel(roadGray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
  convertScaleAbs( grad_x, abs_grad_x );

  /// Gradient Y
  Sobel( roadGray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
  convertScaleAbs( grad_y, abs_grad_y );

  /// Total Gradient (approximate)
  addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, gradImage );

  imshow( "Road Image Sobel", gradImage );

  vector<vector<Point>> contours; // Vector for storing contour
  vector<Vec4i> hierarchy;
  Mat contourImg( roadImage.size().height,roadImage.size().width, CV_8UC1, Scalar(0));
  findContours( gradImage, contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE,Point(0,0) );
  for( int i = 0; i< contours.size(); i++ )
  {
		drawContours(contourImg,contours,i, Scalar(255,255,255), 1, 8, hierarchy);
   }
  imshow( "Road Image contoured", contourImg );
  /*int erode_size =2;  
  Mat erodeElement = getStructuringElement(cv::MORPH_RECT,Size(2 * erode_size + 1, 2* erode_size + 1),Point(erode_size, erode_size) );
	
	Mat erodeImg;
	
	//Apply erosion or dilation on the image
  erode(gradImage,erodeImg,erodeElement); 
  //imshow("Result Road Erode Contour",erodeImg);*/
  waitKey(0);
	return 0;
}

