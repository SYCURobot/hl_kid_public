#include "moves/TemplateBehaviour.h"

#include "moves/Walk.h"
#include "moves/Placer.h"
#include "moves/Approach.h"


#include <rhoban_utils/logging/logger.h>

static rhoban_utils::Logger logger("TemplateBehaviour");


TemplateBehaviour::TemplateBehaviour(Walk *walk, Placer *placer, Approach *approach)
  : walk(walk), placer(placer), approach(approach)
{
  initializeBinding();
  bind->bindNew("state", state)
    ->defaultValue(0);
}

std::string TemplateBehaviour::getName()
{
  return "template_behaviour";
}

void TemplateBehaviour::onStart()
{
  bind->pull();
  startMove("walk", 1.0);
  state = 0;
}

void TemplateBehaviour::onStop()
{
}

void TemplateBehaviour::step(float elapsed)
{
  bind->pull();

  if(state == 0){
    walk->control(false);
    logger.log("Walk disabled.");
  }
  if(state == 1){
    walk->control(true);
    logger.log("Walk enabled.");
  }
  if(state == 2){
    walk->control(true, 100, 0, 0);
    logger.log("Go forward.");
  }
  if(state == 3){
    walk->control(true, 0, 100, 0);
    logger.log("Go left.");
  }
  if(state == 4){
    walk->control(true, 0, 0, 10);
    logger.log("Turn.");
  }



  bind->push();
}
