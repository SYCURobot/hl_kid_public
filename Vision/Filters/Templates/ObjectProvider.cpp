#include "ObjectProvider.hpp"

namespace Vision {
namespace Filters {

ObjectProvider::ObjectProvider(const std::string &name) : Filter(name) {}

const std::vector<double> & ObjectProvider::getObjectsX() const
{
  return objects_x;
}

const std::vector<double> & ObjectProvider::getObjectsY() const
{
  return objects_y;
}

void ObjectProvider::pushObject(double x, double y,
                            const cv::Mat & object_img)
{
  double new_x, new_y;
  new_x = x / object_img.cols;
  new_y = y / object_img.rows;
  objects_x.push_back(new_x);
  objects_y.push_back(new_y);
}

void ObjectProvider::clearObjectsData() {
  objects_x.clear();
  objects_y.clear();
}

}
}
