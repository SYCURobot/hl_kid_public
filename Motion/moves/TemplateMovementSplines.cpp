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
}

void TemplateMovementSplines::step(float elapsed)
{
  bind->pull();


  time += elapsed*splines["remap"].get(time);
  if(time > t_max)
  {
    this->Move::stop();
  }
  else
  {
    setAngle("left_shoulder_pitch", splines["shoulder_pitch"].get(time));
    setAngle("right_shoulder_pitch", splines["shoulder_pitch"].get(time));
  }
  bind->push();
}
