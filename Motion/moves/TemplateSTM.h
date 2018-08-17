#pragma once

#include "STM.h"
#include <string>

class Walk;
class Placer;
class Approach;

class TemplateSTM: public STM
{
    public:
        TemplateSTM(Walk *walk, Placer *placer, Approach *approach);
        std::string getName();
        void onStart();
        void onStop();
        void step(float elapsed);

    protected:
        Walk *walk;
        Placer *placer;
        Approach *approach;

        void enterState(std::string state);
        void exitState(std::string state);
};
