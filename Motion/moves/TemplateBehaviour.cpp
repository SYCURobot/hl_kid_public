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
  startMove("placer");
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
    placer->goTo(0,0,0);
  }
  if(state == 1){
    placer->goTo(2,2,0);
    logger.log("goTo 2 2 0");
  }
  if(state == 2){
    placer->goTo(-2,2,0);
    logger.log("goTo -2 2 0");
  }
  if(state == 3){
    placer->goTo(-2,-2,0);
    logger.log("goTo -2 -2 0");
  }
  if(state == 4){
    placer->goTo(2,-2,0);
    logger.log("goTo 2 -2 0");
  }
  if(state == 5){
    placer->goTo(0,0,90);
    logger.log("goTo 0 0 90");
  }
  if(state == 6){
    placer->goTo(1,0,120);
    logger.log("goTo 1 0 120");
  }

  bind->push();
}
