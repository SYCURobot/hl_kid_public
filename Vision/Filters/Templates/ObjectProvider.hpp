#pragma once

#include "Filters/Filter.hpp"

namespace Vision {
namespace Filters {

/// This class describe the architecture of an object provider and some common code
class ObjectProvider : public Filter {
public:
  ObjectProvider(const std::string &name);

  /// Values are in [0,1]
  const std::vector<double> & getObjectsX() const;
  /// Values are in [0,1]
  const std::vector<double> & getObjectsY() const;

protected:

  /// Automatically rescale provided point to fit the standard
  void pushObject(double x, double y, const cv::Mat & object_img);

  /// Remove previously published data for theobject 
  void clearObjectsData();

  /// Position of the object in the image [0,1]
  std::vector<double> objects_x;

  /// Position of the object in the image [0,1]
  std::vector<double> objects_y;
};
}
}
