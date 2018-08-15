#include "moves/TemplateMovement.h"

#include <rhoban_utils/logging/logger.h>

static rhoban_utils::Logger logger("TemplateMovement");


TemplateMovement::TemplateMovement()
{
  initializeBinding();
  bind->bindNew("parameter_0", rhio_parameter_0)
    ->comment("parameter 0")
    ->defaultValue(true);

  bind->bindNew("parameter_1", rhio_parameter_1, RhIO::Bind::PushOnly)
    ->comment("parameter 1")
    ->defaultValue(2)
    ->minimum(-1)
    ->maximum(10);

  bind->bindNew("parameter_2", rhio_parameter_2, RhIO::Bind::PullOnly)
    ->comment("parameter 2")
    ->defaultValue(1.3);
}

std::string TemplateMovement::getName()
{
  return "template_movement";
}

void TemplateMovement::onStart()
{
  bind->pull();
}

void TemplateMovement::onStop()
{
}

void TemplateMovement::step(float elapsed)
{
  bind->pull();
  bind->push();
}
