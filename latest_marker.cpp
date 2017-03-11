#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include<iostream>
using namespace cv;
using namespace std;
Mat erosion(Mat a,Mat b)
{int r=a.rows,c=a.cols;
  int i,j,k,l;
  for(i=1;i<r-1;i++)
    for(j=1;j<c-1;j++)
      {
	if(a.at<uchar>(i,j)==255)
	 for(k=i-1;k<=i+1;k++)
	    for(l=j-1;l<=j+1;l++)
	      { 
		    if(a.at<uchar>(k,l)==0)
                     b.at<uchar>(i,j)=0;
	 
	      }}
	return b;
}
Mat dilation(Mat a,Mat b)
{int r=a.rows,c=a.cols;
  int i,j,k,l;
  for(i=1;i<r-1;i++)
    for(j=1;j<c-1;j++)
      {
	if(a.at<uchar>(i,j)==0)
	 for(k=i-1;k<=i+1;k++)
	    for(l=j-1;l<=j+1;l++)
	      { 
		    if(a.at<uchar>(k,l)==255)
                     b.at<uchar>(i,j)=255;
	 
	      }}
	return b;
}
int thresh = 100;
Mat rect(Mat a,Mat org )
{int largest_area=0;
    int largest_contour_index=0;
    Rect bounding_rect;

  Mat threshold_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  threshold( a, threshold_output, thresh, 255, THRESH_BINARY );
  findContours( threshold_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
vector<RotatedRect> minRect( contours.size() );
for( int i = 0; i < contours.size(); i++ )
     { minRect[i] = minAreaRect( Mat(contours[i]) );
     }Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( 255,0,0 );
      
       Point2f rect_points[4]; minRect[i].points( rect_points );
       for( int j = 0; j < 4; j++ )
	 {   line( org, rect_points[j], rect_points[(j+1)%4], color, 2, 8 );
		 }
       if(norm(rect_points[0]-rect_points[1])>norm(rect_points[1]-rect_points[2]))//Euclidean distance norm(point1-point2)
    line( org, rect_points[0], rect_points[1], Scalar(0,255,0), 4, 8 );
  else
    line( org, rect_points[1], rect_points[2], Scalar(0,255,0), 4, 8 );
     }
  
  return org;  
  }
int main(  int, char** argv )
{int vmin=70,hmin=0,smin=70,hmax=10,smax=255,vmax=255;
  namedWindow("Mask",WINDOW_AUTOSIZE);
 namedWindow("detect",WINDOW_AUTOSIZE);
  createTrackbar("hmin","Mask",&hmin,180);
createTrackbar("hmax","Mask",&hmax,180);
createTrackbar("smin","Mask",&smin,255);
createTrackbar("smax","Mask",&smax,255);
createTrackbar("vmin","Mask",&vmin,255);
createTrackbar("vmax","Mask",&vmax,255);


  VideoCapture vidi(argv[1]);
  Mat3b bgr,org;
  while(1)
    {
 
  vidi.read(bgr);
 
  org=bgr.clone();
  
    Mat3b hsv;
    cvtColor(bgr, hsv, COLOR_BGR2HSV);

    Mat1b mask1, mask2;
    inRange(hsv, Scalar(hmin,smin,vmin), Scalar(hmax, smax, vmax), mask1);
    Mat1b mask = mask1;
     mask=erosion(mask1,mask1.clone());
     mask=dilation(mask,mask.clone());
     mask=erosion(mask1,mask.clone());
     mask=dilation(mask,mask.clone());
     mask=dilation(mask,mask.clone());
     mask=dilation(mask,mask.clone());
     mask=dilation(mask,mask.clone());
     org= rect(mask,org);  
  imshow("detect", org);
  imshow("Mask",mask1);
   char a= waitKey(50);

   if(a==' ')// 
     break;    }
    return 0;
}
