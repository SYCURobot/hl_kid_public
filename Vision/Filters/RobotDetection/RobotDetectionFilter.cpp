#include "Filters/RobotDetection/RobotDetectionFilter.hpp"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include "CameraState/CameraState.hpp"

#include "rhoban_utils/logging/logger.h"

using namespace rhoban_utils;

static rhoban_utils::Logger out("Super Filter");

namespace Vision {
namespace Filters {

int RobotDetectionFilter::expectedDependencies() const {
  return 4;
}

void RobotDetectionFilter::setParameters() {

  minArea = ParamFloat(100, 0, 2000, ParameterType::PARAM);
  maxArea = ParamFloat(500, 0, 2000, ParameterType::PARAM);

  params()->define<ParamFloat>("minArea", &minArea);
  params()->define<ParamFloat>("maxArea", &maxArea);

}

void RobotDetectionFilter::process() {
  // Get names of dependencies
  const std::string & redRobotName = _dependencies[0];
  const std::string & greenName = _dependencies[1];
  const std::string & clippingName = _dependencies[2];
  const std::string & whiteName = _dependencies[3];
  // Import source matrix
  const cv::Mat & redRobotImg = *(getDependency(redRobotName).getImg());
  const cv::Mat & greenImg = *(getDependency(greenName).getImg());
  const cv::Mat & clippingImg = *(getDependency(clippingName).getImg());
  const cv::Mat & whiteImg = *(getDependency(whiteName).getImg());

  // Set up the detector with parameters.
  cv::SimpleBlobDetector::Params params; 
  params.minDistBetweenBlobs = 1.0;
  params.filterByArea = true;     
  params.filterByCircularity=false;
  params.filterByInertia=false;
  params.filterByConvexity=false;
  params.minThreshold=1;
  params.maxThreshold=100;
  // Tunable parameters
  params.minArea = minArea;      
  params.maxArea = maxArea;

  cv::SimpleBlobDetector detector(params);

  // Detect blobs.
  cv::Mat tmp2(redRobotImg.size(), 0, cv::Scalar(0));
  cv::bitwise_not(redRobotImg, tmp2);

  std::vector<cv::KeyPoint> keypoints;
  detector.detect( tmp2, keypoints);

  // Draw key points
  cv::Mat tmp(redRobotImg.size(), 0, cv::Scalar(0));
  cv::Mat & im_with_keypoints = tmp;
  cv::drawKeypoints(
    tmp2,
    keypoints,
    im_with_keypoints,
    cv::Scalar(0,0,255),
    cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS
  );

  // FloodFill
  cv::Mat greenAndWhite = greenImg + whiteImg;

  cv::Mat clippingImgRightSize;
  cv::resize(clippingImg, clippingImgRightSize, greenImg.size(), 0, 0, cv::INTER_NEAREST);

  cv::Mat notClipping(greenImg.size(), 0, cv::Scalar(0));
  cv::bitwise_not(clippingImgRightSize, notClipping);
  cv::Mat notClippingBorder(greenImg.rows+2, greenImg.cols+2, 0, cv::Scalar(0));
  cv::copyMakeBorder(notClipping, notClippingBorder, 1, 1, 1, 1, cv::BORDER_REPLICATE);
  for(int i=0; i<keypoints.size();i++)
  {
    cv::floodFill(greenAndWhite,notClippingBorder,keypoints[i].pt, cv::Scalar(255), 0, cv::Scalar(), cv::Scalar(),4|cv::FLOODFILL_MASK_ONLY | (128 << 8));
  }

  // Removing clipping from image
  cv::Mat notClippingBorderBis(greenImg.rows+2, greenImg.cols+2, 0, cv::Scalar(0));
  cv::copyMakeBorder(notClipping, notClippingBorderBis, 1, 1, 1, 1, cv::BORDER_REPLICATE);
  cv::Mat robotMask = notClippingBorder - notClippingBorderBis;

  //// CameraState
  int width = greenImg.cols;
  int height = greenImg.rows;
    
  // Bounding box
  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(robotMask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
  std::vector<cv::Rect> boundRec(contours.size());

  cv::Mat contoursAndBoxes(robotMask.rows, robotMask.cols, CV_8UC3, cv::Scalar(0,0,0));
  for(int i=0; i<contours.size(); i++){
    boundRec[i] = cv::boundingRect(contours[i]);
    cv::drawContours( contoursAndBoxes, contours, i, cv::Scalar(255,0,0), 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
    cv::rectangle( contoursAndBoxes, boundRec[i].tl(), boundRec[i].br(), cv::Scalar(0,0,255), 2, 8, 0);

    cv::Point bm = boundRec[i].br() - cv::Point(boundRec[i].width/2, 0);
    cv::circle( contoursAndBoxes, bm, 10, cv::Scalar(0,255,0));

    // Getting the position of the robot in the robot frame
    cv::Point2f robot_position = getCS().robotPosFromImg(bm.x, bm.y, width, height, true);

    std::string msg = "Dist : " + std::to_string(cv::norm(robot_position));
    cv::putText(contoursAndBoxes, msg, boundRec[i].br(), 1, 1, cv::Scalar(255,255,255));
  }

  // Tagging horizon
  cv::Point horizon1, horizon2;
  horizon1.x = 0;
  horizon2.x = width - 1;
  horizon1.y = getCS().getPixelYtAtHorizon(horizon1.x, width, height);
  horizon2.y = getCS().getPixelYtAtHorizon(horizon2.x, width, height);
  // Color in BGR
  cv::line(contoursAndBoxes, horizon1, horizon2, cv::Scalar(255, 0, 0), 2);
  img() = contoursAndBoxes;


}
}
}
