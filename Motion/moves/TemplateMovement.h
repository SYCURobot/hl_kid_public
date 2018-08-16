#pragma once

#include "Move.h"
#include <string>

class TemplateMovement: public Move
{
    public:
        TemplateMovement();
        std::string getName();
        void onStart();
        void onStop();
        void step(float elapsed);

    protected:

        bool rhio_parameter_0;
  float rhio_parameter_1;
  float rhio_parameter_2;
  float angle;
  
  float t;
  
};
