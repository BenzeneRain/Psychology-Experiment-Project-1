#pragma once

#include "stdafx.h"
#include "Screen.h"
#include "TestObject.h"
#include <memory>
using namespace std;

class Trial
{
    public:
        ~Trial();
        static Trial* getInstance();

        enum state {IDLE, PRE_TRIAL_SCENE, MAIN_SCENE, POST_TRIAL_SCENE};

        BOOL startTrial();
        BOOL stepTrial(); 
        BOOL proceedNextScene();
        BOOL recordTrialInfo(TestObject& );
        
        UINT trialID;

   private:
        Trial();
        
        enum state currState;
        
        static auto_ptr<Trial> m_pInstance;
};
