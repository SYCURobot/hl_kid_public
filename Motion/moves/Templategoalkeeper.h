

#include "STM.h"
#include <string>

class Walk;
class Placer;
class Approach;

class Templategoalkeeper: public STM
{
    public:
        Templategoalkeeper(Walk *walk, Placer *placer, Approach *approach);
        std::string getName();
        void onStart();
        void onStop();
        void step(float elapsed);
        float error;
        // double locationNoise;
        // double azimuthNoise;
        // double autoStartX;
        // double autoStartY;
        // double autoStartAzimuth;

    protected:
        Walk *walk;
        Placer *placer;
        Approach *approach;

        void enterState(std::string state);
        void exitState(std::string state);
};
