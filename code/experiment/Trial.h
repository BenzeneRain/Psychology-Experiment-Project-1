#pragma once

#include "stdafx.h"
#include "Screen.h"
#include "TestObject.h"
#include "Conditions.h"
#include <memory>
using namespace std;

class Trial
{
    public:
        Trial(int trialID, cond_t& cond);
        ~Trial();

        enum state {IDLE, PRE_TRIAL_SCENE, MAIN_SCENE, POST_TRIAL_SCENE};

        BOOL startTrial();
        BOOL stepTrial(); 
        BOOL proceedNextScene();
        BOOL recordTrialInfo();

   private:
        enum state currState;
        BOOL finished;
        cond_t& condition;
        UINT trialID;
};
