#include "moves/TemplateMovementSplines.h"

#include <rhoban_utils/util.h>
#include <rhoban_utils/logging/logger.h>
#include <math.h>
#include <rhoban_utils/serialization/json_serializable.h>


using namespace rhoban_utils;
static Logger logger("TemplateMovementSplines");


TemplateMovementSplines::TemplateMovementSplines()
{
  initializeBinding();
}

std::string TemplateMovementSplines::getName()
{
  return "template_movement_with_splines";
}

void TemplateMovementSplines::onStart()
{
  bind->pull();

  time = 0.0;
  times= 0;

  try { 
    splines = Function::fromFile("new_movement.json");
  } catch (const JsonParsingError & exc) {
    logger.error("%s", exc.what());
  }

  t_max = 0;
  for (auto &entry :splines) {
      double duration = entry.second.getXMax();
      if (duration > t_max) {
          t_max = duration;
      }
  }


}

void TemplateMovementSplines::onStop()
{
    splines.clear();
}

void TemplateMovementSplines::step(float elapsed)
{
  bind->pull();


  time += elapsed*splines["remap"].get(time);
  if(time > t_max)
  {
    time=0.0;
    times += 1;
    //this->Move::stop();
  }
  else if(times == 3)
  {
    this->Move::stop();
  }
  else
  {
    setAngle("right_hip_pitch", splines["hip_pitch"].get(time));
    setAngle("right_knee", splines["right_knee"].get(time));
  }
  bind->push();
}
