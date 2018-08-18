#include "moves/AutonomousBehaviour.h"

#include "moves/Head.h"
#include "moves/StandUp.h"
#include "moves/Walk.h"


#include <services/DecisionService.h>
#include <services/LocalisationService.h>

#include <rhoban_utils/logging/logger.h>

#include <set>

#define STATE_WAITING  "waiting"
#define STATE_BEHAVIOUR  "behaviour"
#define STATE_CHANGE_BEHAVIOUR  "change_behaviour"
#define STATE_STANDUP  "standing_up"
#define STATE_HANDLED  "handled"

static rhoban_utils::Logger logger("AutonomousBehaviour");
static std::set<std::string> behaviours ={
  "playing","template_stm"
};


AutonomousBehaviour::AutonomousBehaviour(Walk *walk, StandUp *standup, Head *head)
  : walk(walk), standup(standup), head(head)
{
  initializeBinding();
  // State
  bind->bindNew("state", STM::state, RhIO::Bind::PushOnly)
    ->comment("State of the AutonomousBehaviour STM");
  bind->bindNew("behaviour", behaviour)
    ->comment("Behaviour of the AutonomousBehaviour STM");
}

std::string AutonomousBehaviour::getName()
{
  return "autonomous_behaviour";
}

void AutonomousBehaviour::onStart()
{
  bind->pull();
  behaviour = "playing";
  standup_try = 0;
  setState(STATE_WAITING);
  head->setDisabled(false);
  walk->control(false);
}

void AutonomousBehaviour::onStop()
{
  head->setDisabled(true);
  stopMove(behaviour);
  setState(STATE_WAITING);
}

void AutonomousBehaviour::step(float elapsed)
{
  auto &decision = getServices()->decision;
  previous_behaviour = behaviour;
  bind->pull();

  if(behaviours.count(behaviour) == 0){
    behaviour = previous_behaviour;
  }

  // Detect if the robot is being handled
  if (decision->handled) {
    setState(STATE_HANDLED);
  } else if (state == STATE_HANDLED) {
    setState(STATE_WAITING);
  }

  t += elapsed;

  // Fall recovery start from waiting state
  if (state == STATE_WAITING && decision->isFallen) {
    logger.log("Robot has fallen, standing up");
    setState(STATE_STANDUP);
  }

  // If robot was standing up standup has finished, jump to Waiting state
  if (state == STATE_STANDUP && standup->over) {
    logger.log("Standup has finished, back to Waiting");
    setState(STATE_WAITING);
  }

  // If robot was and it fell, go to waiting (then it will go to standup)
  if (decision->isFallen && (state == STATE_BEHAVIOUR)) {
    logger.log("Robot has fallen, going to wait");
    setState(STATE_WAITING);
  }

  // Exiting buffer state (waiting)
  if (state == STATE_WAITING && !decision->handled && !decision->isFallen) {
    setState(STATE_BEHAVIOUR);
  }

  // Spam stop orders to walk when waiting or handled
  if (state == STATE_WAITING || state == STATE_HANDLED) {
    walk->control(false);
  }

  if (state == STATE_BEHAVIOUR && previous_behaviour != behaviour){
    setState(STATE_CHANGE_BEHAVIOUR);
  }

  if (state == STATE_CHANGE_BEHAVIOUR){
    setState(STATE_BEHAVIOUR);
  }
  bind->push();
}


void AutonomousBehaviour::enterState(std::string state)
{ 
    t = 0;

    Head * head = (Head*)getMoves()->getMove("head");
    // Not scanning only if the robot is handled
    if (state == STATE_HANDLED) {
        head->setDisabled(true);
    } else {
        head->setDisabled(false);
    }

    // Handling StandUp related stuff
    if (state == STATE_STANDUP) {
        standup->setLayDown(false);
        stopMove("walk", 0.3);
        startMove("standup", 0.0);
        standup->trying = standup_try;
    } else {
        startMove("walk", 1.0);
        walk->control(false);
    }

    if (state == STATE_BEHAVIOUR) {
      startMove(behaviour, 0.0);
    }

    if (state == STATE_CHANGE_BEHAVIOUR){
      logger.log("Behaviour changed to %s", behaviour.c_str());
      stopMove(previous_behaviour, 0.0);
      previous_behaviour = behaviour;
    }
}

void AutonomousBehaviour::exitState(std::string state)
{
    auto &decision = getServices()->decision;

    // After standing up:
    // - Apply a fallReset on filters
    // - Stop the standup move
    if (state == STATE_STANDUP) {
        auto loc = getServices()->localisation;
        loc->fallReset();
        stopMove("standup", 0.0);
        // Used to slow next standup if first one failed
        if (decision->isFallen) {
            standup_try++;
        } else {
            standup_try = 0;
        }
    }

    // Stop main move when leaving behaviour
    if (state == STATE_BEHAVIOUR) {
      stopMove(behaviour, 0.0);
    }
}

