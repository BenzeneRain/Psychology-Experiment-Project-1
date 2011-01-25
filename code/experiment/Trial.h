#pragma once

#include "stdafx.h"
#include "Screen.h"
#include "TestObject.h"
#include "Conditions.h"
#include <memory>
using namespace std;

struct trialResult_struct
{
    trialResult_struct::trialResult_struct(float rFps, float rDuration, string& rTrialName):
        fps(rFps), duration(rDuration), trialName(rTrialName) {}
    const float fps;
    const float duration;
    const string trialName;
};
typedef struct trialResult_struct trialResult_t;

class Trial
{
    public:
        Trial(int trialID, cond_t& cond);
        ~Trial();

        enum state {IDLE, PRE_TRIAL_SCENE, MAIN_SCENE, POST_TRIAL_SCENE, FINISHED};

        BOOL startTrial();
        BOOL stepTrial(); 
        BOOL proceedNextScene();
        BOOL recordTrialInfo();

   private:
        enum state currState;
        BOOL finished;
        cond_t& condition;
        UINT trialID;
        
        vector<trialResult_t *> _results;
};
