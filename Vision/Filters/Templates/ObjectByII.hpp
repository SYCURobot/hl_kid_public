#pragma once

#include "Filters/Filter.hpp"

namespace Vision {
namespace Filters {

 /**
 * Uses an idea based on the approach proposed by Berlin United (spl team) at RoHOW 2017 to
 * identify the best 'n' regions of interest
 *
 * Input:
 *   - dependency 1
 *   - dependency 2
 *
 * Output:
 *   - An image of (rows, cols) where pixels are colored in red or blue depending on their value
 */
class ObjectByII : public Filter {
public:
  ObjectByII() : Filter("ObjectByII") {}

  virtual std::string getClassName() const override;
  virtual int expectedDependencies() const override;

protected:
  virtual void process() override;

  virtual void setParameters() override;

  cv::Mat getHeatMap(const cv::Mat & scores, double minScore, double maxScore) const;

private:
  /// Return the half width of the ROI given the object size.
  /// It takes boundaryFactor and maxBoundaryThickness in account
  double getBoundaryHalfWidth(float size);
  /// Return the patch associated to the boundary part of the ball at the given point
  cv::Rect_<float> getBoundaryPatch(int x, int y, float size);
  cv::Rect_<float> getInnerPatch(int x, int y, float size);
  cv::Rect_<float> getLeftBoundaryPatch(int x, int y, float size);
  cv::Rect_<float> getRightBoundaryPatch(int x, int y, float size);
  cv::Rect_<float> getTopBoundaryPatch(int x, int y, float size);
  cv::Rect_<float> getBottomBoundaryPatch(int x, int y, float size);

  /// Return the score of the patch given the provided integralImage
  /// If "I" is the image from which the integralImage was computed,
  /// the function returns the sum of the values of each pixel
  /// inside the rectangle of "I".
  double getPatchScore(const cv::Rect & patch,
                       const cv::Mat & integralImage);

  /// Return the score of a pixel, it can use the patchScore of several patchs.
  ///  Look BallByII for example.
  double getCandidateScore(int center_x, int center_y, double size,
                           const cv::Mat & y_img, const cv::Mat & green_img);

  /// img should be CV_32SC1
  /// draw pixels in [start_x,end_x[ * [start_y,end_y[
  void fillScore(cv::Mat & img, int score,
                 int start_x, int end_x,
                 int start_y, int end_y);

  /// Number of rows in the result image
  int rows;

  /// Number of columns in the result image;
  int cols;

  /// Usual size ratio between object size and size of the ROI
  ParamFloat boundaryFactor;

  /// The maximal thickness of the border area around the object (usually grass)
  ParamFloat maxBoundaryThickness;

  /// Minimal size of the object in pixels for the size of image used
  ParamFloat minSize;

  /// Minimal score for considering that region of interest is acceptable
  /// Reminder: Scores are in [-255,510] (2 * B - I)
  ParamFloat minScore;

  ParamFloat greenWeight, yWeight;

  /// Maximal number of ROI generated by the filter
  ParamInt maxRois;

  /// Since using a high resolution Integral Image helps to detect balls which
  /// are far away, a decimationRate is used to compute the score function only
  /// at an interval given by decimationRate
  ParamInt decimationRate;

  /// 0: No heatMap produced
  ParamInt tagLevel;

  /// If enabled, a local search is used nearby every selected candidate
  ParamInt useLocalSearch;
};
}
}


