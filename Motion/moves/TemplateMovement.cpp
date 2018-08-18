#include "moves/TemplateMovement.h"

#include <rhoban_utils/logging/logger.h>
#include <math.h>
static rhoban_utils::Logger logger("TemplateMovement");


TemplateMovement::TemplateMovement()
{
  initializeBinding();
  bind->bindNew("parameter_0", rhio_parameter_0)
    ->comment("parameter 0")
    ->defaultValue(true);

  bind->bindNew("amp", rhio_parameter_1, RhIO::Bind::PullOnly)
    ->comment("move amplitude")
    ->defaultValue(10)
    ->minimum(0)
    ->maximum(100);

  bind->bindNew("freq", rhio_parameter_2, RhIO::Bind::PullOnly)
    ->comment("move frequency")
    ->defaultValue(1.3);
  bind->bindNew("angle", angle,RhIO::Bind::PushOnly)
    ->comment("motor angle")
    ->defaultValue(0);

}

std::string TemplateMovement::getName()
{
  return "template_movement";
}

void TemplateMovement::onStart()
{
  bind->pull();
  t=0.0;
}

void TemplateMovement::onStop()
{
}

void TemplateMovement::step(float elapsed)
{

  bind->pull();
  t+=elapsed;
  // rhio_parameter_1=t;
  setTorqueLimit("right_shoulder_pitch", 1.0);
  float pitch=sin(2.0*M_PI*rhio_parameter_2*t)*rhio_parameter_1;
  angle=pitch;
  Move::setAngle("right_shoulder_pitch", pitch);
  bind->push();
}
