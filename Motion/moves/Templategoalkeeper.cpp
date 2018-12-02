#include "moves/Templategoalkeeper.h"

#include "moves/Walk.h"
#include "moves/Placer.h"
#include "moves/Approach.h"
#include "moves/Playing.h"

#include <services/LocalisationService.h>
#include <rhoban_geometry/point.h>

#include <rhoban_utils/logging/logger.h>

#define STATE_INITIAL "go_to_initial"
#define STATE_GO_TO_BALL "go_to_ball"
#define STATE_GO_TO_HOME "go_to_home"
#define STATE_ALING_BALL "aling_with_ball" 
#define STATE_KICK_BALL  "attack"      // if ball near in near area, it should be attack
//#define STATE_WAITING    "waiting"    //if ball far, it should be waiting

static rhoban_utils::Logger logger("Templategoalkeeper");


Templategoalkeeper::Templategoalkeeper(Walk *walk, Placer *placer, Approach *approach)
  : walk(walk), placer(placer), approach(approach)
{
    // locationNoise = 0.3;
    // azimuthNoise = 10;
    // autoStartX=-3.5;
    // autoStartY=-3.5;
    // autoStartAzimuth=90;
  initializeBinding();
    // State
    bind->bindNew("state", STM::state, RhIO::Bind::PushOnly)
        ->comment("Templategoalkeeper state"); 
    // bind->bindNew("autoStartX", autoStartX, RhIO::Bind::PullOnly)
    //     ->comment("Start of the robot during ready phase [m]");
    // bind->bindNew("autoStartY", autoStartY, RhIO::Bind::PullOnly)
    //     ->comment("Start of the robot during ready phase [m]");
    // bind->bindNew("autoStartAzimuth", autoStartAzimuth, RhIO::Bind::PullOnly)
    //     ->comment("Initial orientation of the robot during ready phase [deg]");   
}

std::string Templategoalkeeper::getName()
{
  return "template_goalkeeper";
}

void Templategoalkeeper::onStart()
{
  bind->pull();
  startMove("placer");
  startMove("walk", 1.0);
  
  //logger.log("Putting me on the floor in initial state");
  //auto initiallocation=loc->customFieldReset(autoStartX, autoStartY, locationNoise,autoStartAzimuth, azimuthNoise);
  // auto initiallocation = loc->customFieldReset(-3.5, -3, 0.3, 10, 10);
  // auto robot = loc->getFieldPos();
  //  if (robot.x==initiallocation.x && robot.y==initiallocation.y){
  //                                    setState(STATE_GO_TO_HOME);
  //                                  }                              
  setState(STATE_GO_TO_HOME);
}

void Templategoalkeeper::onStop()
{
  stopMove("placer");
  stopMove("walk", 1.0);
}

void Templategoalkeeper::step(float elapsed)
{
  bind->pull();
  auto loc = getServices()->localisation;
   // if(state == STATE_INITIAL){ 
   //    if (!placer->arrived){
   //        placer->goTo(-3,-3,0);
   //        logger.log("initiallocation");
   //    }
   //    else{
   //          setState(STATE_GO_TO_HOME);
   //    }         
   // }

  if(state == STATE_GO_TO_BALL){
    if(!placer->arrived){
      auto ball = loc->getBallPosField();
      placer->goTo(ball.x, ball.y, 0);    
      setState(STATE_KICK_BALL);
    }
    else{
      setState(STATE_GO_TO_HOME);
    }
  }
  if(state == STATE_KICK_BALL){ 
      logger.log("STATE_KICK_BALL"); 
      float error= 0.1;
      auto robot = loc->getFieldPos();
      auto ball = loc->getBallPosField();
      if (robot.x <= -3 - error){
        startMove("playing");
        placer->arrived=false;
        logger.log("robot first playing");
        if (placer->arrived && ball.x <= 0 -error){
        setState(STATE_ALING_BALL);
        }
        else if (ball.x >= 2 - error){
        setState(STATE_GO_TO_HOME);  
        }
        else if (robot.x <= -3 - error){
        startMove("playing");
        logger.log("robot second playing");
        }
      }
      else{
        setState(STATE_GO_TO_HOME);
      }
  }
  // else{
  //     setState(STATE_GO_TO_HOME);
  //     logger.log("else playing");
  // }

  if(state == STATE_GO_TO_HOME){
    if(!placer->arrived){
      placer->goTo(-4.5, 0, 0);
    }
    else{
      setState(STATE_GO_TO_BALL);
    }
  }
  bind->push();
}


void Templategoalkeeper::enterState(std::string state)
{ 
  auto loc = getServices()->localisation;

  if(state == STATE_GO_TO_BALL){
    auto ball = loc->getBallPosField();
    placer->goTo(ball.x, ball.y, 0);
    placer->arrived = false;
  }
  if(state == STATE_GO_TO_HOME){
    placer->goTo(-6,0,0);
    placer->arrived = false;
  }
  if(state == STATE_KICK_BALL){
    startMove("playing");
    logger.log("enterstate playing");
  }
   if(state == STATE_INITIAL){
    placer->goTo(-3,-3,0);
    placer->arrived = false;
  }
    if(state == STATE_ALING_BALL){
    startMove("walk", 1.0);;
    logger.log("STATE_ALING_BALL");
  }
}

void Templategoalkeeper::exitState(std::string state)
{
  if(state == STATE_GO_TO_BALL){
    //stopMove("placer");
  }
  if(state == STATE_GO_TO_HOME){
   // stopMove("placer");
  }
  if(state == STATE_KICK_BALL){
    //stopMove("playing");
  }
  if(state == STATE_INITIAL){
    //stopMove("placer");
  }
  if(state == STATE_ALING_BALL){
  }
}

