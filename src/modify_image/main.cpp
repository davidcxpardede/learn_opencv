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

In this example, we will perform 5x5 Gaussian kernel
 */

 Mat image_blurred_with_5x5_kernel;
 GaussianBlur(image, image_blurred_with_5x5_kernel, Size(5, 5), 0);

 bool isSuccess = imwrite("./data/stars_blurred.jpg", image_blurred_with_5x5_kernel); //write the image to a file as JPEG 
 //bool isSuccess = imwrite("./data/stars_blurred.png", image_blurred_with_5x5_kernel); //write the image to a file as PNG
 if (isSuccess == false)
 {
  cout << "Failed to save the image" << endl;
  cin.get(); //wait for a key press
  return -1;
 }

 cout << "Image is succusfully saved to a file" << endl;

 return 0;
}