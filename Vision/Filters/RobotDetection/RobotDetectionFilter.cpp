#include "Filters/RobotDetection/RobotDetectionFilter.hpp"

#include <opencv2/imgproc/imgproc.hpp>

namespace Vision {
namespace Filters {

void RobotDetectionFilter::setParameters() {

  min0 = ParamInt(124, 0, 255, ParameterType::PARAM);
  max0 = ParamInt(124, 0, 255, ParameterType::PARAM);
  min1 = ParamInt(124, 0, 255, ParameterType::PARAM);
  max1 = ParamInt(124, 0, 255, ParameterType::PARAM);
  min2 = ParamInt(124, 0, 255, ParameterType::PARAM);
  max2 = ParamInt(124, 0, 255, ParameterType::PARAM);

  params()->define<ParamInt>("min0", &min0);
  params()->define<ParamInt>("max0", &max0);
  params()->define<ParamInt>("min1", &min1);
  params()->define<ParamInt>("max1", &max1);
  params()->define<ParamInt>("min2", &min2);
  params()->define<ParamInt>("max2", &max2);
}

void RobotDetectionFilter::process() {
  cv::Mat src = *(getDependency().getImg());

  cv::Mat temp(src.size(), 0, cv::Scalar(0)); // Unsigned char mat
  cv::Scalar lowLimit =  cv::Scalar(min0, min1, min2);
  cv::Scalar highLimit = cv::Scalar(max0, max1, max2);

  inRange(src, lowLimit, highLimit, temp);

  cv::Mat res(src.size(), 0, cv::Scalar(0)); // Unsigned char mat


  img() = temp;
}
}
}
