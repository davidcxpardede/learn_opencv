#include <iostream>
#include <opencv2/opencv.hpp>
#include <list>
#include <set>
#include <algorithm>
#include <tuple>
using namespace cv;
using namespace std;

// create necessery datasets
typedef tuple<int /*x*/,int /*y*/> PixelPosT;
typedef set<PixelPosT> PixelPosSetT;
typedef list<PixelPosT> PixelPosListT;

/**
 * Removes all white neighbours arond pixel from whitePixels
 * if they exist and adds them to pixelsToBeProcessed and to
 * pixelsinCluster.
 further reading: https://www.lost-infinity.com/night-sky-image-processing-part-3-star-clustering/
 */
void getAndRemoveNeighbours(PixelPosT inCurPixelPos, PixelPosSetT * inoutWhitePixels, 
                       PixelPosListT * inoutPixelsToBeProcessed,
                       PixelPosListT * outPixelCluster){
  const size_t _numPixels = 8, _x = 0, _y = 1;
  const int offsets[_numPixels][2] = { { -1, -1 }, { 0, -1 }, { 1, -1 },
                                       { -1, 0 },              { 1, 0 },
                                       { -1, 1 }, { 0, 1 }, { 1, 1 } };
  
  for (size_t p = 0; p < _numPixels; ++p) {
    PixelPosT curPixPos(
                        std::get<0>(inCurPixelPos) + offsets[p][_x],
                        std::get<1>(inCurPixelPos) + offsets[p][_y]
    );
    PixelPosSetT::iterator itPixPos = inoutWhitePixels->find(curPixPos);
 
    if (itPixPos != inoutWhitePixels->end()) {
      const PixelPosT & curPixPos = *itPixPos;
      inoutPixelsToBeProcessed->push_back(curPixPos);
      outPixelCluster->push_back(curPixPos);
      // Remove white pixel from "white set" since it has
      // been now processed
      inoutWhitePixels->erase(itPixPos);
    }
  }
  return;
}
 

void cluster_stars(Mat & image_threshold, vector<PixelPosListT> * outRecognizedClusters) {
  PixelPosSetT whitePixels;

  for(int j=0;j<image_threshold.rows;j++) 
  {
    for (int i=0;i<image_threshold.cols;i++)
    {
      if((int)image_threshold.at<uchar>(i,j)==255){
        whitePixels.insert(whitePixels.end(), PixelPosT(i, j));
      }
      
    }
  }
 
  // Iterate over white pixels as long as set is not empty
  while (whitePixels.size()) {
    PixelPosListT pixelCluster;
    PixelPosListT pixelsToBeProcessed;
 
    PixelPosSetT::iterator itWhitePixPos = whitePixels.begin();
    pixelsToBeProcessed.push_back(*itWhitePixPos);
    whitePixels.erase(itWhitePixPos);
 
    while(! pixelsToBeProcessed.empty()) {
      PixelPosT curPixelPos = pixelsToBeProcessed.front();
      getAndRemoveNeighbours(curPixelPos, & whitePixels, & pixelsToBeProcessed, & pixelCluster);
      pixelsToBeProcessed.pop_front();
    }
 
    // Finally, append the cluster
    outRecognizedClusters->push_back(pixelCluster);
  }
}

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
  cout << "Image successfully opened!" << endl;
  
  // Perform otsu threshold
  Mat image_threshold;
  threshold(image, image_threshold, 0, 255, THRESH_OTSU);

  // save enhanced image
  bool isSuccess = imwrite("./data/result/stars_enhanced.jpg", image_threshold); //write the image to a file as JPEG 
  if (isSuccess == false){
    cout << "Failed to save the image" << endl;
    cin.get(); //wait for a key press
    return -1;
  } else {
    cout << "Image is succesfully saved to a file" << endl;
  }
  
  // start clustering
  vector<PixelPosListT> recognizedPixelClusters;
  try {
    cluster_stars(image_threshold, & recognizedPixelClusters);
    cout << "Recognized " << recognizedPixelClusters.size() << " stars..." << endl;
  } catch(...) {
    cerr << "Problem occured!" << endl;
  }

  return 0;
}