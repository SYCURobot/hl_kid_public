#pragma once

#include "Move.h"
#include <string>

class Walk;
class Placer;
class Approach;

class TemplateBehaviour: public Move
{
    public:
        TemplateBehaviour(Walk *walk, Placer *placer, Approach *approach);
        std::string getName();
        void onStart();
        void onStop();
        void step(float elapsed);

    protected:
        Walk *walk;
        Placer *placer;
        Approach *approach;


        bool rhio_parameter_0;
        int rhio_parameter_1;
        float rhio_parameter_2;

        int state;
};
