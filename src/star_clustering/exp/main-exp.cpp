// Include the C++ STL that will be used for this program
#include <iostream>
#include <opencv2/opencv.hpp>
#include <list>
#include <set>
#include <algorithm>
#include <tuple>
using namespace cv;
using namespace std;

// Create the Necessary Datasets
typedef tuple<int /*x*/,int /*y*/> PixelPosT; // A tuple with two int arguments called PixelPosT
typedef set<PixelPosT> PixelPosSetT;          // A set with a tuple argument called PixelPosSetT
typedef list<PixelPosT> PixelPosListT;        // A list with a tuple argument called PixelPosListT

/**
 * Removes all white neighbours arond pixel from whitePixels
 * if they exist and adds them to pixelsToBeProcessed and to
 * pixelsinCluster.
 further reading: https://www.lost-infinity.com/night-sky-image-processing-part-3-star-clustering/
 */

// Declare the First Function
  // The getAndRemoveNeighbours function has four arguments, consisting of a tuple, a set, and two tuples.
  void getAndRemoveNeighbours(PixelPosT inCurPixelPos, PixelPosSetT * inoutWhitePixels, 
                       PixelPosListT * inoutPixelsToBeProcessed,
                       PixelPosListT * outPixelCluster){

    // Declare three variables with the type of const size_t                        
    const size_t _numPixels = 8, _x = 0, _y = 1;
    
    // Declare an integer array with the size of _numPixels (8) and 2
    const int offsets[_numPixels][2] = { { -1, -1 }, { 0, -1 }, { 1, -1 },
                                       { -1, 0 },              { 1, 0 },
                                       { -1, 1 }, { 0, 1 }, { 1, 1 } };
  
    for (size_t p = 0; p < _numPixels; ++p) {
      // Declare a tuple called curPixPos with two int arguments
      PixelPosT curPixPos(
                        std::get<0>(inCurPixelPos) + offsets[p][_x],  // The first argument is the first element of inCurPixelPos plus the element of offsets [p] and [_x]
                        std::get<1>(inCurPixelPos) + offsets[p][_y]   // The second argument is the second element of inCurPixelPos plus the element of offsets [p] and [_y]
      );  // For each p (from 0 to 7), the values of the arguments of curPixPos are stored as desribed above
      // Declare a PixelPosSetT iterator called itPixPos
      // itPixPos is an iterator that finds the values of curPixPos and store it as inoutWhitePixels (pointer)
      PixelPosSetT::iterator itPixPos = inoutWhitePixels->find(curPixPos);

      // Create an if statement to check whether itPixPos has a value (whether the value of inOutWhitePixels is found inside the curPixPos)
      if (itPixPos != inoutWhitePixels->end()) {
        // Store the pointer of itPixPos in the curPixPos
        const PixelPosT & curPixPos = *itPixPos;
        // Add a new element with the values of curPixPos in the backmost of inoutPixelstoBeProcessed
        inoutPixelsToBeProcessed->push_back(curPixPos);
        // Add a new element with the values of curPixPos in the backmost of outPixelCluster
        outPixelCluster->push_back(curPixPos);
        // Remove white pixel from "white set" since it has been now processed
        inoutWhitePixels->erase(itPixPos);
      } // If otherwise itPixPos has no valid value (the value of inOutWhitePixels is not found inside the curPixPos), do nothing
    }
    return;
  }
 
// Declare the Second Function
  // Create a function called cluster_stars with two argument called image_threshold (matrix) and outRecognizedClusters (vector list)
  void cluster_stars(Mat & image_threshold, vector<PixelPosListT> * outRecognizedClusters) {
  
    // Declare a set called whitePixels
    PixelPosSetT whitePixels;

    // Create a for loop with the number of rows of image_threshold as iterators
    for(int j=0;j<image_threshold.rows;j++) 
    {
      // Create another for loop with the number of columns of image_threshold as iterators
      for (int i=0;i<image_threshold.cols;i++)
      {
        // Check for each value of image_threshold in the corresponding coordinate (i,j)
        // Check whether that value equals 255 or not (255 means it is white enough, a.k.a. the brightness is sufficient)
        if((int)image_threshold.at<uchar>(i,j)==255){
          // If the value equals 255, new values are added (within the range of the end of whitePixels and the values of PixelPost at i,j)
          whitePixels.insert(whitePixels.end(), PixelPosT(i, j));
        }
      
      }
    }
 
    // Iterate over white pixels as long as set is not empty
    // Create a while statement that returns true as long as it is not false (0)
    while (whitePixels.size()) {
      // Declare two lists called pixelCluster and pixelsToBeProcessed
      PixelPosListT pixelCluster;
      PixelPosListT pixelsToBeProcessed;
      
      // Declare an iterator called itWhitePixPos that points the first element of whitePixels
      PixelPosSetT::iterator itWhitePixPos = whitePixels.begin();
      // Add another element at the end of pixelsToBeProcessed with the value of itWhitePixPos
      pixelsToBeProcessed.push_back(*itWhitePixPos);
      // Erase the value from whitePixels with the position of itWhitePixPos (beginning)
      whitePixels.erase(itWhitePixPos);

      // Create a while loop that executes if the pixelsToBeProcessed is not empty
      while(! pixelsToBeProcessed.empty()) {
        // Declare a tuple called curPixelPos that equals the first element of pixelsToBeProcessed
        PixelPosT curPixelPos = pixelsToBeProcessed.front();
        // Calling the getAndRemoveNeighbours with four arguments
        getAndRemoveNeighbours(curPixelPos, & whitePixels, & pixelsToBeProcessed, & pixelCluster);
        // Erase the first element of pixelsToBeProcessed
        pixelsToBeProcessed.pop_front();
      }
 
      // Finally, append the cluster
      // Add pixelCluster as the endmost element of outRecognizedClusters
      outRecognizedClusters->push_back(pixelCluster);
    }
  }

// Write the main function with two arguments, argc and argv
int main(int argc, char** argv)
{
  // Read the image file and store it in image (matrix)
  // The image file is located in the data folder with the name specified below
  // The image file is of the type IMREAD_GRAYSCALE (an 8-bit grayscale image)
  Mat image = imread("./data/stars_ori.jpg", IMREAD_GRAYSCALE);

  // Check for failure
  if (image.empty())
  {
    cout << "Could not open or find the image" << endl;
    cin.get(); //wait for any key press
    return -1;
  }
  cout << "Image successfully opened!" << endl;
  
  // Perform otsu threshold
  // Declare a matrix called image_threshold
  Mat image_threshold;
  // Using threshold to identify the image_threshold with five arguments
  threshold(image, image_threshold, 0, 255, THRESH_OTSU);

  // Save enhanced image
  // Declare a boolean called isSuccess
  // write the image to a file as JPEG 
  bool isSuccess = imwrite("./data/result/stars_enhanced.jpg", image_threshold); 


  if (isSuccess == false){
    cout << "Failed to save the image" << endl;
    cin.get(); //wait for a key press
    return -1;
  } else {
    cout << "Image is succesfully saved to a file." << endl;
  }
  
  // Star Clustering
  // Declare a vector called recognizedPixelClusters with PixelPosLisT as argument
  vector<PixelPosListT> recognizedPixelClusters;
  try {
    // Call the second function with two arguments
    cluster_stars(image_threshold, & recognizedPixelClusters);
    // Display the number of stars (size of recognizedPixelClusters vector)
    cout << "Recognized " << recognizedPixelClusters.size() << " stars." << endl;
  } catch(...) {
    cerr << "Problem occured!" << endl;
  }

  return 0;
}