#pragma once

#include "STM.h"
#include <string>

class StandUp;
class Walk;
class TemplateMovement: public STM
{
    public:
        TemplateMovement(Walk *walk, Placer *placer, Approach *approach);
        std::string getName();
        void onStart();
        void onStop();
        void step(float elapsed);
        bool isFallen();
        bool isHandled();

    protected:
        Walk *walk;
        Placer *placer;
        Approach *approach;

        void enterState(std::string state);
        void exitState(std::string state);

        bool rhio_parameter_0;
        int rhio_parameter_1;
        float rhio_parameter_2;
};
