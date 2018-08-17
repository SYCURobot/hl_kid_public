#pragma once

#include "Move.h"
#include <rhoban_utils/spline/function.h>
#include <string>

class Legup: public Move
{
  public:
    Legup();
    std::string getName();
    void onStart();
    void onStop();
    void step(float elapsed);

  protected:

    float time;
    float t_max;
    std::map<std::string, rhoban_utils::Function> splines;
};
