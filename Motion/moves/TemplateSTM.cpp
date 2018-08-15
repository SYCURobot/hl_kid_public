#include "moves/TemplateSTM.h"

#include "moves/Walk.h"
#include "moves/Placer.h"
#include "moves/Approach.h"


#include <rhoban_utils/logging/logger.h>

#define STATE_STARTING "starting"
#define STATE_STOPING "stoping"

static rhoban_utils::Logger logger("TemplateSTM");


TemplateSTM::TemplateSTM(Walk *walk, Placer *placer, Approach *approach)
  : walk(walk), placer(placer), approach(approach)
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

std::string TemplateSTM::getName()
{
  return "template_stm";
}

void TemplateSTM::onStart()
{
  bind->pull();
  setState(STATE_STARTING);
}

void TemplateSTM::onStop()
{
  setState(STATE_STOPING);
}

void TemplateSTM::step(float elapsed)
{
  bind->pull();
  bind->push();
}


void TemplateSTM::enterState(std::string state)
{ 
  if(state == STATE_STARTING){
  }
  if(state == STATE_STOPING){
  }
}

void TemplateSTM::exitState(std::string state)
{
  if(state == STATE_STARTING){
  }
  if(state == STATE_STOPING){
  }
}

