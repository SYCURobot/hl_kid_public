#include "moves/Legup.h"

#include <rhoban_utils/util.h>
#include <rhoban_utils/logging/logger.h>
#include <math.h>
#include <rhoban_utils/serialization/json_serializable.h>


using namespace rhoban_utils;
static Logger logger("Legup");

Legup::Legup()
{
  initializeBinding();
}

std::string Legup::getName()
{
  return "Legup";
}

void Legup::onStart()
{
  bind->pull();

  time = 0.0;

  try { 
    splines = Function::fromFile("Legup.json");
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

void Legup::onStop()
{
  splines.clear();
}

void Legup::step(float elapsed)
{
  bind->pull();


  time += elapsed*splines["remap"].get(time);
 //   setAngle("right_hip_pitch", splines["right_hip_pitch"].get(time));
 //   setAngle("right_knee", splines["right_knee_pitch"].get(time));
  if(time > t_max)
  {
    this->Move::stop();
  }
  else
  {

    setAngle("right_hip_pitch", splines["right_hip_pitch"].get(time));
    setAngle("right_knee", splines["right_knee"].get(time));
 //   setAngle("right_hip_pitch", splines["hip_pitch"].get(time));
 //   setAngle("left_hip_pitch", splines["hip_pitch"].get(time));
 //   setAngle("right_knee", splines["knee_pitch"].get(time));
 //   setAngle("left_knee", splines["knee_pitch"].get(time));

  }
  bind->push();
}
