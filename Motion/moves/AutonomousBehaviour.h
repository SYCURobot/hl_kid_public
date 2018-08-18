#pragma once

#include "STM.h"
#include <string>

/// Sta autonomously without listening to the referee
/// Just use automated standup and stop playing when handled
class StandUp;
class Walk;
class Head;
class AutonomousBehaviour : public STM
{
    public:
        AutonomousBehaviour(Walk *walk, StandUp *standup, Head* head);
        std::string getName();
        void onStart();
        void onStop();
        void step(float elapsed);
        bool isFallen();
        bool isHandled();

    protected:
        Walk *walk;
        StandUp *standup;
        Head* head;

        std::string behaviour;
        std::string previous_behaviour;

        float t;
        int standup_try;
        double timeSinceVisionInactive;

        void applyGameState();

        void enterState(std::string state);
        void exitState(std::string state);
};
