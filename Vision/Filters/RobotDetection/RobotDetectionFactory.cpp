#include "RobotDetectionFactory.hpp"

#include "RobotDetectionFilter.hpp"

#include "../FilterFactory.hpp"

namespace Vision {
namespace Filters {

void registerRobotDetectionFilters(FilterFactory * ff) {
  ff->registerBuilder("RobotDetectionFilter", [](){return std::unique_ptr<Filter>(new RobotDetectionFilter());});
}

}
}
