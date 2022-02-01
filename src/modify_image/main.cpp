#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
 // Read the image file
 Mat image = imread("./data/stars_ori.jpg", IMREAD_GRAYSCALE);

 // Check for failure
 if (image.empty())
 {
  cout << "Could not open or find the image" << endl;
  cin.get(); //wait for any key press
  return -1;
 }

 cout << "Image successfully openned" << endl;

 /*
 Make changes to the image as necessary
 e.g.  
  1. Change brightness/contrast of the image
  2. Smooth/Blur image
  3. Crop the image
  4. Rotate the image
  5. Draw shapes on the image
 In this example, we perform otsu threshold
 */

  Mat image_threshold;
  threshold(image, image_threshold, 0, 255, THRESH_OTSU);

 bool isSuccess = imwrite("./data/result/stars_enhanced.jpg", image_threshold); //write the image to a file as JPEG 
 //bool isSuccess = imwrite("./data/result/stars_enhanced.jpg", image_threshold); //write the image to a file as PNG
 if (isSuccess == false)
 {
  cout << "Failed to save the image" << endl;
  cin.get(); //wait for a key press
  return -1;
 }

 cout << "Image is succusfully saved to a file" << endl;

 /*
 Perform access value in matrix of images
 */
 int bright_pixel = 0, pixelValue;
 for(int j=0;j<image_threshold.rows;j++) 
  {
    for (int i=0;i<image_threshold.cols;i++)
    {
      pixelValue = (int)image_threshold.at<uchar>(i,j);
      if(pixelValue=255){
        bright_pixel += 1;
      }
      
    }
  }

  cout << "Found "<< bright_pixel << " pixels with max intensity"<<endl;

 return 0;
}