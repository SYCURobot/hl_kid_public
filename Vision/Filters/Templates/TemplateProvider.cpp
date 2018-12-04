#include "TemplateProvider.hpp"

namespace Vision {
namespace Filters {

TemplateProvider::TemplateProvider(const std::string &name) : Filter(name) {}

const std::vector<double> & TemplateProvider::getGoalsX() const
{
  return goals_x;
}

const std::vector<double> & TemplateProvider::getGoalsY() const
{
  return goals_y;
}

void TemplateProvider::pushGoal(double x, double y,
                            const cv::Mat & goal_img)
{
  double new_x, new_y;
  new_x = x / goal_img.cols;
  new_y = y / goal_img.rows;
  goals_x.push_back(new_x);
  goals_y.push_back(new_y);
}

void TemplateProvider::clearGoalsData() {
  goals_x.clear();
  goals_y.clear();
}

}
}
