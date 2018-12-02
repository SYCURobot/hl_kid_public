#pragma once

#include <vector>
#include "Filters/Filter.hpp"

namespace Vision {
namespace Filters {

/**
 * TemplateOpenCV
 *
 * Minimal code to make a filter using opencv.
 *
 */

/**
 * Config file
    {
        "class name" : "TemplateOpenCV",
        "content" : {
            "name" : "templateOpenCVExample",
            "dependencies" : ["source"],
            "paramInts" : {
                "paramInt1" : 40,
                "paramInt2" : 40
            },
            "paramFloat" : {
                "paramFloat1" : 40,
                "paramFloat2" : 40
            }
        }
    }
 */
class TemplateOpenCV : public Filter {
public:
  TemplateOpenCV() : Filter("TemplateOpenCV") {}

  virtual std::string getClassName() const override { return "TemplateOpenCV"; }

  /**
   * Examples of int and float parameters.
   */
  ParamInt paramInt1, paramInt2;
  ParamFloat paramFloat1 , paramFloat2;

protected:
  /**
   * @Inherit
   */
  virtual void process() override;

  virtual void setParameters() override;
};
}
}
