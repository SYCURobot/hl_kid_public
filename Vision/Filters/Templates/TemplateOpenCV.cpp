#include "Filters/Templates/TemplateOpenCV.hpp"

#include <opencv2/imgproc/imgproc.hpp>

namespace Vision {
namespace Filters {

void TemplateOpenCV::setParameters() {

  // Int parameters
  // value, minValue, maxValue
  paramInt1 = ParamInt(124, 0, 255);
  paramInt2 = ParamInt(124, 0, 255);
  params()->define<ParamInt>("paramInt1", &paramInt1);
  params()->define<ParamInt>("paramInt2", &paramInt2);

  // Float parameters
  // value, minValue, maxValue
  paramFloat1 = ParamFloat(124, 0, 255);
  paramFloat2 = ParamFloat(124, 0, 255);
  params()->define<ParamFloat>("paramFloat1", &paramFloat1);
  params()->define<ParamFloat>("paramFloat2", &paramFloat2);
}

void TemplateOpenCV::process() {
  cv::Mat src = *(getDependency().getImg());

  cv::Mat temp(src.size(), 0, cv::Scalar(0)); // Unsigned char mat

  /**
   * Modify temp.
   */

  img() = temp;
}
}
}
