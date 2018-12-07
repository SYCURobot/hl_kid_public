#pragma once

#include <vector>
#include "Filters/Filter.hpp"

namespace Vision {
namespace Filters {

/**
 * RobotDetectionFilter 
 *
 * Minimal code to make a filter using opencv.
 *
 */

/**
 * Config file
    {
        "class name" : "RobotDetectionFilter",
        "content" : {
            "name" : "robotFilterDetectionExample",
            "dependencies" : ["source"],
            "paramInts" : {
                "max0" : 124,
                "min0" : 124,
                "max1" : 124,
                "min1" : 124,
                "max2" : 124,
                "min2" : 124 
            }
        }
    }
 */
class RobotDetectionFilter: public Filter {
public:
  RobotDetectionFilter() : Filter("RobotDetectionFilter") {}

  virtual std::string getClassName() const override { return "RobotDetectionFilter"; }

  ParamFloat minArea, maxArea;

  virtual int expectedDependencies() const override;

protected:
  /**
   * @Inherit
   */
  virtual void process() override;

  virtual void setParameters() override;
};
}
}
